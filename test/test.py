import cv2
import numpy as np
import serial
from infer_engine import InferEngine

def xywh2xyxy(x):
    """将 [x, y, w, h] 转换为 [x1, y1, x2, y2]"""
    y = np.copy(x)
    y[:, 0] = x[:, 0] - x[:, 2] / 2
    y[:, 1] = x[:, 1] - x[:, 3] / 2
    y[:, 2] = x[:, 0] + x[:, 2] / 2
    y[:, 3] = x[:, 1] + x[:, 3] / 2
    return y

def py_cpu_nms(dets, thresh):
    """纯 Python 实现的非极大值抑制 (NMS)"""
    x1 = dets[:, 0]
    y1 = dets[:, 1]
    x2 = dets[:, 2]
    y2 = dets[:, 3]
    areas = (y2 - y1 + 1) * (x2 - x1 + 1)
    scores = dets[:, 4]
    res = []
    index = scores.argsort()[::-1]
    
    while index.size > 0:
        i = index[0]
        res.append(i)
        x11 = np.maximum(x1[i], x1[index[1:]])
        y11 = np.maximum(y1[i], y1[index[1:]])
        x22 = np.minimum(x2[i], x2[index[1:]])
        y22 = np.minimum(y2[i], y2[index[1:]])

        w = np.maximum(0, x22 - x11 + 1)
        h = np.maximum(0, y22 - y11 + 1)

        overlaps = w * h
        iou = overlaps / (areas[i] + areas[index[1:]] - overlaps)

        idx = np.where(iou <= thresh)[0]
        index = index[idx + 1]
    
    return res

def filter_box(org_box, conf_thres, iou_thres):
    """通过置信度阈值过滤框并应用 NMS"""
    org_box = np.squeeze(org_box)
    conf = org_box[..., 4] > conf_thres
    box = org_box[conf == True]

    cls_conf = box[..., 5:]
    cls = np.argmax(cls_conf, axis=1)
    all_cls = np.unique(cls)
    
    output = []
    for curr_cls in all_cls:
        curr_cls_box = box[cls == curr_cls]
        curr_cls_box[:, 5] = curr_cls
        curr_cls_box = xywh2xyxy(curr_cls_box)
        keep = py_cpu_nms(curr_cls_box, iou_thres)
        output.extend(curr_cls_box[keep])
    
    return np.array(output)

def process_frame(frame, engine, CLASSES, ser, conf_thres=0.25, iou_thres=0.45):
    imag = cv2.resize(frame, (640, 640))
    img = imag[:, :, ::-1].transpose(2, 0, 1)  # BGR 转 RGB 并从 HWC 转 CHW
    
    image = np.expand_dims(img, axis=0)
    format = de.PixelFormat.DE_PIX_FMT_RGB888_PLANE
    shape = (image.shape[0], image.shape[1], image.shape[2], image.shape[3])
    
    data = [(format, shape, image)]
    output = engine.predict(data)[0]
    
    outbox = filter_box(output, conf_thres, iou_thres)
    
    for o in outbox:
        x1, y1, x2, y2 = map(int, o[:4])
        score = o[4]
        classes = int(o[5])
        
        cv2.rectangle(imag, (x1, y1), (x2, y2), (255, 0, 0), 2)
        text = "%s:%.2f" % (CLASSES[classes], score)
        cv2.putText(imag, text, (x1, y1), cv2.FONT_HERSHEY_SIMPLEX, 0.75, (0, 0, 255), 2)
        
        # 计算中心点
        cx = (x1 + x2) // 2
        cy = (y1 + y2) // 2
        center_data = f"{cx},{cy}\n"
        
        # 通过串口发送中心点数据
        ser.write(center_data.encode())
    
    return imag

if __name__ == "__main__":
    print("InferEngine 示例开始...")

    net_file = "/sktpu_tvm_sdk_1000/aftermodel/y5_onnx_test/net.bin"
    model_file = "/sktpu_tvm_sdk_1000/aftermodel/y5_onnx_test/model.bin"
    
    engine = InferEngine(net_file, model_file, max_batch=1)
    
    CLASSES = (
        "red", "green", "blue"
    )

    # 初始化串口,波特率
    ser = serial.Serial('/dev/ttyAMA0', 19200, timeout=1)  

    cap = cv2.VideoCapture(0)

    if not cap.isOpened():
        print("无法打开摄像头")
        exit()

    while True:
        ret, frame = cap.read()
        if not ret:
            print("无法读取帧")
            break
        
        processed_frame = process_frame(frame, engine, CLASSES, ser)
        
        cv2.imshow('视频识别', processed_frame)
        
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()
    ser.close()

    engine.profile()
