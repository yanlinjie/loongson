#include "24cxx.h" 

extern axis_Set axis; //���꼯

void read_axis(void){
	u16 rough_first_x, rough_first_y,  Staging_first_x, Staging_first_y;
		// ��ά�봦
		axis.x_code = 770;	
    axis.y_code = 140;
		// ת�̴�
    axis.x_Raw = 1470; // 700	
    axis.y_Raw = 100;   // -45
  	// �ּӹ�����һ��
		rough_first_x = 1991;   // X	
		rough_first_y = 898;   // Y
	  // �ݴ�����һ��
		Staging_first_x = 1234;	 // X	
		Staging_first_y = 1812;   // Y
	
	if( rough_order   == "RGB" ){
	   axis.x_C =  rough_first_x;      
	   axis.y_C_r =  rough_first_y; 
		 axis.y_C_g = axis.y_C_r + 150;
		 axis.y_C_b = axis.y_C_r + 300;
	}
	else if( rough_order == "RBG" ){
	   axis.x_C =  rough_first_x;      
	   axis.y_C_r =  rough_first_y; 
		 axis.y_C_b = axis.y_C_r + 150;
		 axis.y_C_g = axis.y_C_r + 300;
	}
	else if( rough_order == "BGR" ){
	   axis.x_C =  rough_first_x;      
	   axis.y_C_b =  rough_first_y; 
		 axis.y_C_g = axis.y_C_b + 150;
		 axis.y_C_r = axis.y_C_b + 300;
	}
	else if( rough_order == "BRG" ){
	   axis.x_C =  rough_first_x;      
	   axis.y_C_b =  rough_first_y; 
		 axis.y_C_r = axis.y_C_b + 150;
		 axis.y_C_g = axis.y_C_b + 300;	
	
	}
	else if( rough_order == "GBR" ){
		 axis.x_C =  rough_first_x;      
	   axis.y_C_g =  rough_first_y; 
		 axis.y_C_b = axis.y_C_g + 150;
		 axis.y_C_r = axis.y_C_g + 300;
	}
	else if( rough_order == "GRB" ){
		 axis.x_C =  rough_first_x;      
	   axis.y_C_g =  rough_first_y; 
		 axis.y_C_r = axis.y_C_g + 150;
		 axis.y_C_b = axis.y_C_g + 300;	
	}
	///////////////////////////////////////////////////////////////////////////////
		if( Staging_order   == "RGB" ){
		 axis.x_D_r =  Staging_first_x;      
	   axis.y_D =  Staging_first_y;
		 axis.x_D_g = axis.x_D_r - 150;
		 axis.x_D_b = axis.x_D_r - 300;
	}
	else if( Staging_order == "RBG" ){
		 axis.x_D_r =  Staging_first_x;      
	   axis.y_D =  Staging_first_y;
		 axis.x_D_b = axis.x_D_r - 150;
		 axis.x_D_g = axis.x_D_r - 300;
	}
	else if( Staging_order == "BGR" ){
		 axis.x_D_b =  Staging_first_x;      
	   axis.y_D =  Staging_first_y;
		 axis.x_D_g = axis.x_D_b - 150;
		 axis.x_D_r = axis.x_D_b - 300;
	}
	else if( Staging_order == "BRG" ){
		 axis.x_D_b =  Staging_first_x;      
	   axis.y_D =  Staging_first_y;
		 axis.x_D_r = axis.x_D_b - 150;
		 axis.x_D_g = axis.x_D_b - 300;
	}
	else if( Staging_order == "GBR" ){
		 axis.x_D_g =  Staging_first_x;      
	   axis.y_D =  Staging_first_y;
		 axis.x_D_b = axis.x_D_g - 150;
		 axis.x_D_r = axis.x_D_g - 300;
	}
	else if( Staging_order == "GRB" ){
		 axis.x_D_g =  Staging_first_x;      
	   axis.y_D =  Staging_first_y;
		 axis.x_D_r = axis.x_D_g - 150;
		 axis.x_D_b = axis.x_D_g - 300;
	}
}








