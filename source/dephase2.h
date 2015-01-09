//-------------------------------------------------------------------------------------------------------
// VST Plug-Ins SDK
// Version 2.4		$Date: 2006/11/13 09:08:27 $
//
// Category     : VST 2.x SDK Samples
// Filename     : again.h
// Created by   : Steinberg Media Technologies
// Description  : Stereo plugin which applies Gain [-oo, 0dB]
//
// © 2006, Steinberg Media Technologies, All Rights Reserved
//-------------------------------------------------------------------------------------------------------

#ifndef __dephase2__
#define __dephase2__

#include "public.sdk/source/vst2.x/audioeffectx.h"
#include "calde.h"
#include "singen.h"

enum
{	// Parameters Tags
	kDeg = 0,
	kBal,
	kVit,
	kByby,
	kCli,
	kSin,
	kOut,

	kNumParams
};

//-------------------------------------------------------------------------------------------------------
class Dephase2 : public AudioEffectX
{
public:
	Dephase2 (audioMasterCallback audioMaster);
	~Dephase2 ();

	void setSampleRate(float p_srate);	
	// Processing
	virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
	virtual void processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames);

	// Program
	//virtual void setProgram (VstInt32 program);
	virtual void setProgramName (char* name);
	virtual void getProgramName (char* name);
	//virtual bool getProgramNameIndexed (VstInt32 category, VstInt32 index, char* text);

	// Parameters
	virtual void setParameter (VstInt32 index, float value);
	virtual float getParameter (VstInt32 index);
	virtual void getParameterLabel (VstInt32 index, char* label);
	virtual void getParameterDisplay (VstInt32 index, char* text);
	virtual void getParameterName (VstInt32 index, char* text);

	virtual bool getEffectName (char* name);
	virtual bool getVendorString (char* text);
	virtual bool getProductString (char* text);
	virtual VstInt32 getVendorVersion ();

protected:
	
	float fDeg, vDeg;
	float fBal, vBal;
	float fByby, vByby;
	float fVit, vVit;
	float fCli, vCli;
	float fSin, vSin;
	float fOut, vOut;

	//DephaseProgram* programs;
	//VstInt32 channelPrograms[16];

	float scaler;

	float phase1;
	float phase2;

	float prec1;
	float prec2;
	
	Calde freq1;
	Calde freq2;

	SinGen sin1;
	SinGen sin2;


	//float* buf1;
	//float* buf2;

	char programName[kVstMaxProgNameLen + 1];
};

#endif
