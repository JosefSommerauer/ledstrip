#ifndef LEDSTRIPE_H
#define LEDSTRIPE_H

struct T_RGB {
  T_RGB() : R(0), G(0), B(0) {}
  T_RGB(int r, int g, int b) : R(r), G(g), B(b) {}
  int R;
  int G;
  int B;
};

struct T_YUV {
  int Y;
  int Cr;
  int Cb;
};

class LedStripe {
	public:
	  LedStripe(int red, int green, int blue);
	  void SetColor(T_RGB color);
	  void SetColor(int red, int green, int blue);
	  void SetColor(int red, int green, int blue, int brightness);
	  void SetColorKeepBrightness(int red, int green, int blue);
	  
	  void Off();
	  void On();
	  
	  void SetBrightness(int val);
	  void IncBrightness();
	  void DecBrightness();
	  
	  T_RGB getRGB();
	  T_YUV getYUV();
  
	private:
		int red_pin;
		int green_pin;
		int blue_pin;
		
		int mRed;
		int mGreen;
		int mBlue;
		
		int length = 1000;
		int mBrightness;
};


#endif
