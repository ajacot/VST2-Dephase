//-------------------------------------------------------------------------------------------------------
// VST Plug-Ins SDK
// Version 2.4		$Date: 2006/11/13 09:08:27 $
//
// Category     : VST 2.x SDK Samples
// Filename     : again.cpp
// Created by   : Steinberg Media Technologies
// Description  : Stereo plugin which applies Gain [-oo, 0dB]
//
// © 2006, Steinberg Media Technologies, All Rights Reserved
//-------------------------------------------------------------------------------------------------------

#include "dephase2.h"

//#include "log.h"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new Dephase2 (audioMaster);
}

//-------------------------------------------------------------------------------------------------------
Dephase2::Dephase2 (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1, kNumParams)	// 1 program, 1 parameter only
{
	//Log("--------------\ninit\n");
	setNumInputs (2);		// stereo in
	setNumOutputs (2);		// stereo out
	setUniqueID ('ase2');	// identify
	canProcessReplacing ();	// supports replacing output
	//canDoubleReplacing ();	// supports double precision processing

	//Log("end init\n");

	vDeg = 1.0f;	// saw
	vBal = 1.0f;
	vByby = 0.0f;
	vVit = 1.0f;
	vCli = 0.0f;
	vSin = 1.0f;
	vOut = 1.0f;

	//Log("set parameters\n");

	setParameter(kDeg, vDeg);
	setParameter(kBal, vBal);
	setParameter(kVit, vVit);
	setParameter(kCli, vCli);
	setParameter(kSin, vSin);
	setParameter(kOut, vOut);

	phase1 = 0.0f;
	phase2 = 0.0f;

	prec1 = 0.0f;
	prec2 = 0.0f;

	setSampleRate(44100);

	vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name
	//Log("end end init\n---------------------- \n");
}

//-------------------------------------------------------------------------------------------------------
Dephase2::~Dephase2 ()
{
	// nothing to do here
}

//--------------------------------------------------------------------------------------------------------
void Dephase2::setSampleRate(float p_srate)
{
	//Log("set sampleRate\n");
	sampleRate = p_srate;
	scaler=(float)((double)2.0 / (double)sampleRate);
	freq1.setSampleRate(p_srate);
	freq2.setSampleRate(p_srate);
}
//-------------------------------------------------------------------------------------------------------
void Dephase2::setProgramName (char* name)
{
	//Log("setProgramName\n");
	vst_strncpy (programName, name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void Dephase2::getProgramName (char* name)
{
	//Log("getProgramName\n");
	vst_strncpy (name, programName, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void Dephase2::setParameter (VstInt32 index, float value)
{
	//Log("setParameter\n");
	switch (index)
	{
		case kDeg :		vDeg = value;fDeg = value*value*10;		break;
		case kBal :		vBal = value;fBal = value;				break;
		case kByby :	vByby = value;fByby = 1-(1-value)*(1-value);	break;
		case kVit :		vVit = value;fVit = value*value*20;			break;
		case kCli :		vCli = value;fCli = value;				break;
		case kSin :		vSin = value;fSin = value;				break;
		case kOut :     vOut = value;fOut = value*2;				break;
	}
	freq1.update(fDeg, fBal, fVit);
	freq2.update(fDeg, fBal, fVit);
	//fDeg = value*value*10;
}

//-----------------------------------------------------------------------------------------
float Dephase2::getParameter (VstInt32 index)
{
	//Log("getParameter\n");
	float v = 0;

	switch (index)
	{
		case kDeg :		v = vDeg;	break;
		case kBal :		v = vBal;	break;
		case kByby :	v = vByby;	break;
		case kVit :		v = vVit;	break;
		case kCli :		v = vCli;	break;
		case kSin :		v = vSin;	break;
		case kOut :     v = vOut;	break;
	}
	return v;
}

//-----------------------------------------------------------------------------------------
void Dephase2::getParameterName (VstInt32 index, char* label)
{
	//Log("getParameterName\n");
	switch (index)
	{
		case kDeg :    strcpy (label, "Deg");			break;
		case kBal : strcpy (label, "Balance");			break;
		case kByby : strcpy (label, "Integrator");		break;
		case kVit : strcpy (label, "modVitesse");		break;
		case kCli : strcpy (label, "clipSound");		break;
		case kSin : strcpy (label, "Sinus");		break;
		case kOut :      strcpy (label, "Volume");		break;
	}
	//vst_strncpy (label, "byby", kVstMaxParamStrLen);
}

//-----------------------------------------------------------------------------------------
void Dephase2::getParameterDisplay (VstInt32 index, char* text)
{
	//Log("getParameterDisplay\n");
	switch (index)
	{
		case kDeg :		float2string (fDeg, text, kVstMaxParamStrLen);		break;
		case kBal :		float2string (fBal, text, kVstMaxParamStrLen);		break;
		case kByby :	float2string (fByby, text, kVstMaxParamStrLen);		break;
		case kVit :		float2string (fVit, text, kVstMaxParamStrLen);		break;
		case kCli :		float2string (fCli, text, kVstMaxParamStrLen);		break;
		case kSin :		float2string (fSin, text, kVstMaxParamStrLen);		break;
		case kOut :     dB2string (fOut, text, kVstMaxParamStrLen);			break;
	}
	//float2string (fDeg, text, kVstMaxParamStrLen);
}

//-----------------------------------------------------------------------------------------
void Dephase2::getParameterLabel (VstInt32 index, char* label)
{
	//Log("getParameterLabel\n");
	switch (index)
	{
		case kDeg :		strcpy (label, "pieds");	break;
		case kBal :		strcpy (label, "juste");	break;
		case kByby :	strcpy (label, "grüss Got");	break;
		case kVit :		strcpy (label, "jambes");	break;
		case kCli :		strcpy (label, "von");	break;
		case kSin :		strcpy (label, "mein");	break;
		case kOut :     strcpy (label, "dB");		break;
	}
	//vst_strncpy (label, "cutoff", kVstMaxParamStrLen);
}

//------------------------------------------------------------------------
bool Dephase2::getEffectName (char* name)
{
	//Log("getEffectName\n");
	vst_strncpy (name, "Dephase2", kVstMaxEffectNameLen);
	return true;
}

//------------------------------------------------------------------------
bool Dephase2::getProductString (char* text)
{
	//Log("getProductString\n");
	vst_strncpy (text, "Dephase2", kVstMaxProductStrLen);
	return true;
}

//------------------------------------------------------------------------
bool Dephase2::getVendorString (char* text)
{
	//Log("getVendorString\n");
	vst_strncpy (text, "Bobby", kVstMaxVendorStrLen);
	return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 Dephase2::getVendorVersion ()
{ 
	//Log("getVendorVersion\n");
	return 666; 
}

//-----------------------------------------------------------------------------------------
void Dephase2::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
	//Log("process replacing\n");
	
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];
    float* out1 = outputs[0];
    float* out2 = outputs[1];

	//memset(buf1, 0, sampleFrames*sizeof(float));
	//memset(buf2, 0, sampleFrames*sizeof(float));

	//freq1.processAcc(buf1, in1, sampleFrames);
	//freq2.processAcc(buf2, in2, sampleFrames);

		//sljedeca linija koda se koristi umjesto modulo 
	//operatora jer se % operator generalno smatra skupim
    while (--sampleFrames >= 0)
    {
		prec1 *= fByby;
		prec2 *= fByby;
		prec1 += (freq1.calcule(*in1++))*scaler;
		prec2 += (freq2.calcule(*in2++))*scaler;
		phase1 += prec1*(1.2f-fByby)*(1.15f-fByby)*5;
		phase2 += prec2*(1.2f-fByby)*(1.15f-fByby)*5;

		sin1.setFreq(prec1*(1.2f-fByby)*(1.15f-fByby)*80000.0f);
		sin2.setFreq(prec2*(1.2f-fByby)*(1.15f-fByby)*80000.0f);

		//phase1 += 440*scaler*100;
		//phase2 += 220*scaler*100;

		while (phase1 >= (float)2.0||phase1<(float)0.0){
		phase1 += (float)(phase1<0) ? 2.0f :-2.0f;
		//phase1 = 0.0f;
		}

		while (phase2 >= (float)2.0||phase2<(float)0.0){
		phase2 += (float)(phase2<0) ? 2.0f :-2.0f;
		//phase2 = 0.0f;
		}
		*out1 = sin1.calcule(0.35f/sampleRate)*fSin*20.0f;
		*out2 = sin2.calcule(0.35f/sampleRate)*fSin*20.0f;

		*out1 += ((phase1 <= 1.0) ? 2*phase1-1 : 3-2*phase1)*fCli;
		*out2 += ((phase2 <= 1.0) ? 2*phase2-1 : 3-2*phase2)*fCli;

		(*out1++) *= fOut;
		(*out2++) *= fOut;
    }
}

//-----------------------------------------------------------------------------------------
void Dephase2::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames)
{

	//Log("processDoubleReplacing\n");
	/*
    double* in1  =  inputs[0];
    double* in2  =  inputs[1];
    double* out1 = outputs[0];
    double* out2 = outputs[1];
	double dOut = fOut;

    while (--sampleFrames >= 0)
    {
        (*out1++) += 0;//(*in1++)*dOut;
        (*out2++) += 0;//(*in2++)*dOut;
    }*/
}
