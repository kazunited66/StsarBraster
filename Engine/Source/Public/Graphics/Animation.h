#pragma once

class Texture;

struct AnimationParams {
	AnimationParams() {
		FrameWidth = FrameHeight = 0; 
		fps = 0.0f;
		StartFrame = EndFrame = 0;
		MaxFrames = 0;


	}

	//width and height of each frame 
	unsigned int  FrameWidth, FrameHeight;

	//Frames per second 
	float fps; 

	//start and end frame that the animation will play 
	unsigned int StartFrame, EndFrame;
    
    //amount of frames in the sprite sheet 
	unsigned int MaxFrames;

};
class Animation {
public: 
	Animation();
	~Animation();

	//imports the animation and adds animation parameters 
	bool CreateAnimation(const char* PathToFile, AnimationParams* Params = nullptr); 
	
	//upgate the animation logis each frame
	void Update(float DeltaTime);

	void SetPosition(int x, int y);

	void SetRotation(float Angle);

	void SetScale(float Scale, float y);
	//set the visibility of animation 
	void SetVisible(float IsVisible);
private:
	Texture* m_TextureRef;

	//store the animation data
	AnimationParams* m_AnimParams;

	//the  current frame of animation 
	unsigned int m_CurrentFrame;
	
	//how much time has passed from last frame 
	float m_FrameTimer;
};