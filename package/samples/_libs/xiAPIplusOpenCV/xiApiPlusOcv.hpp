

//-------------------------------------------------------------------------------------------------------------------
// xiAPIplus header file - created from gen_xiAPIplus_h_file.xslt
// XIMEA Application Programming Interface Object Oriented Approach
//-------------------------------------------------------------------------------------------------------------------

#pragma once

#if defined (_WIN32)
#include <xiApi.h>       // Windows
#else
#include <m3api/xiApi.h> // Linux, OSX
#endif

#include <stdio.h>
#include <opencv2/core.hpp> 
#include <opencv2/highgui/highgui.hpp>
#include <stdint.h>
#include <vector>
// debug support macros
#define DBFIN  "xAPIp:"
#define DBFOUT "res: "
#define xiAPIPlusDP(x) {if (is_debug_enabled) {printf x;}}
#define CheckCamHandle(place) {xiAPIPlusDP((DBFIN "%s ... ",place));if (!camera_handle) CheckResult(XI_INVALID_HANDLE, place);}
#define CheckCamHandleInt(place,i) {xiAPIPlusDP((DBFIN "%s(%d) ... ",place,i));if (!camera_handle) CheckResult(XI_INVALID_HANDLE, place);}

// forward declarations
/**
* ReportException
* Throws xiAPIplus_Exception for specific XI_RETURN error code and location in code.
* @param[in] res			XI_RETURN code returned from function.
* @param[in] location		location in code, where return value was reported.
*/
void ReportException(XI_RETURN res, const char* location);

// -----------------------------------------------
// xiAPIplus
// -----------------------------------------------

class xiAPIplus
{
public:
 	/**
	* GetNumberOfConnectedCameras
	* Returns the number of all discovered devices.
	* @return Number of discovered devices.
	*/
	unsigned long GetNumberOfConnectedCameras();

};

class xiAPIplus_Exception
{
public:
 	/**
	* xiAPIplus_Exception
	* xiAPIplus_Exception class constructor.
	* @param[in] XI_RETURN					return value from xiAPI function.
    * @param[in] desc						error description string.
	*/
	xiAPIplus_Exception(XI_RETURN code, char* desc)
	{
		description = desc;
		error_code = code;
	}

 	/**
	* GetErrorNumber
	* Returns cached XI_RETURN error code value.
	* @return XI_RETURN value stored in exception.
	*/
	XI_RETURN GetErrorNumber() {return error_code;}

 	/**
	* GetDescription
	* xiAPIplus_Exception class constructor.
	* @param[in] name					pointer to buffer to store description info text.
    * @param[in] name_max_size			length of provided buffer for description.
	*/
	void GetDescription(char* name, size_t name_max_size);

 	/**
	* PrintError
	* Print exception error code and description to standard output (stdout).
	*/
	void PrintError();

private:

	char*     description;			//!< exception description string
	XI_RETURN error_code;			//!< exception error code
};

// -----------------------------------------------
// class xiAPIplus - Image
// -----------------------------------------------

class xiAPIplus_Image
{
public:
	xiAPIplus_Image();
	virtual ~xiAPIplus_Image();

 	/**
	* GetDataFormat
	* Returns Format of image data get from GetImage.
	* @return Data format of captured image.
	*/
	XI_IMG_FORMAT GetDataFormat() {return image.frm;}

 	/**
	* GetPixels
	* Returns pointer to pixel data.
	* @return Pointer to pixel data.
	*/
	unsigned char* GetPixels() { return (unsigned char*)image.bp; }

 	/**
	* GetPixelsFloat
	* Returns pointer to pixel data casted as pointer to float.
	* @return Pointer to pixel data casted as pointer to float.
	*/
	float* GetPixelsFloat() { return (float*)image.bp; }

 	/**
	* GetWidth
	* Returns width of incoming image.
	* @return Width of incoming image.
	*/
	int GetWidth() {return image.width;}

 	/**
	* GetHeight
	* Returns height of incoming image.
	* @return Height of incoming image.
	*/
	int GetHeight() {return image.height;}

 	/**
	* GetPadding_X
	* Returns number of extra bytes provided at the end of each line to facilitate image alignment in buffers.
	* @return Number of extra bytes provided at the end of each line to facilitate image alignment in buffers.
	*/
	int GetPadding_X();

 	/**
	* GetXI_IMG
	* Returns pointer to internal XI_IMG instance.
	* @return Pointer to internal XI_IMG instance.
	*/
	XI_IMG* GetXI_IMG();

 	/**
	* GetBytesPerPixel
	* Returns number of bytes per pixel based on image data format.
	* @return Number of bytes per pixel.
	*/
	int GetBytesPerPixel();

 	/**
	* GetFrameNumber
	* Returns frame number. On some cameras it is reset by exposure, gain, downsampling change, auto exposure (AEAG).
	* @return Frame number.
	*/
	int GetFrameNumber(){ return image.nframe; };

 	/**
	* GetPixelsArraySize
	* Returns size of image in bytes.
	* @return Size of image in bytes.
	*/
	int GetPixelsArraySize() { return ((image.width + GetPadding_X()) * image.height * GetBytesPerPixel()); }

 	/**
	* GetBitCount
	* Returns pixel bit count based on image data format.
	* @return Pixel bit count.
	*/
	int GetBitCount();

 	/**
	* GetBitCount
	* Make new copy of existing image. Should be deleted by caller after finishing of use.
	* @param[in] image_copy					Pointer to xiAPIplus_Image to contain the copied data.
	*/
	virtual void CopyTo(xiAPIplus_Image* image_copy);

 	/**
	* IsCopy
	* Returns internal flag to signal, whether pixel buffer was allocated locally (true) or by xiAPI (false).
	* @return Internal flag, true when pixel buffer allocated locally, false otherwise.
	*/
	bool IsCopy() {return is_copy;}

 	/**
	* SetCopy
	* Set internal flag to identify, whether pixel buffer was allocated locally or by xiAPI.
	* @param[in] en					New value of the internal 'is_copy' flag.
	*/
	void SetCopy(bool en) { is_copy = en; }

private:

	XI_IMG image;		// internal instance if xiAPI structure containing information about incoming image.
	bool is_copy;		// =1 if image bitmap is allocated, 0= if it is area in memory, that is not allocated in API
};

// -----------------------------------------------
// class xiAPIplus - Camera
// -----------------------------------------------

class xiAPIplus_Camera
{
public:

	xiAPIplus_Camera();
	~xiAPIplus_Camera();

	/**
	* GetNumberOfConnectedCameras
	* Returns the number of all discovered devices.
	* @return Number of discovered devices.
	*/
	unsigned long GetNumberOfConnectedCameras();

	/**
	* OpenFirst
	* Open the first connected camera.
	*/
	void OpenFirst();

	/**
	* OpenByID
	* Open the camera by index of the connected device.
	* @param[in] id								index of the device.
	*/
	void OpenByID(const unsigned long id);

	/**
	* OpenBySN
	* Open camera by its serial number.
	* @param[in] serial_number					serial number of camera to be opened.
	*/
	void OpenBySN(const char* serial_number);

	/**
	* OpenBySN
	* Open camera by its hardware path.
	* @param[in] device_path					device path of camera to be opened.
	*/
	void OpenByPath(const char* device_path);

	/**
	* OpenByUserID
	* Open camera by its custom user ID.
	* @param[in] user_id						user ID of camera to be opened.
	*/
	void OpenByUserID(const char* user_id);

	/**
	* OpenByUserID
	* Open camera by its hardware location path.
	* @param[in] location						hardware location path of camera to be opened.
	*/
	void OpenByLocation(const char* location);

	/**
	* Close
	* Closes the camera and releases allocated resources.
	*/
	void Close();

	/**
	* EnableDebug
	* Enable printing of internal xiApiPlus checks into standard output (stdout).
	*/
	void EnableDebug() { is_debug_enabled = true; }

	/**
	* DisableDebug
	* Disable printing of internal xiApiPlus checks into standard output (stdout).
	*/
	void DisableDebug() { is_debug_enabled = false; }

	/**
	* IsDebugEnabled
	* Report state of internal xiApiPlus debugging flag.
	* @return State of internal xiApiPlus debugging flag.
	*/
	bool IsDebugEnabled() {return is_debug_enabled;}

	/**
	* StartAcquisition
	* Begins the work cycle and starts data acquisition from the camera.
	*/
	void StartAcquisition();

	/**
	* StopAcquisition
	* Ends the work cycle of the camera, stops data acquisition and deallocates internal image buffers.
	*/
	void StopAcquisition();

	/**
	* IsAcquisitionActive
	* Return the current state of data acquisition, true if running, false if stopped.
	* @return State of data acquisition, true if running, false if stopped.
	*/
	bool IsAcquisitionActive();

	/**
	* SetNextImageTimeout_ms
	* Set internal data acquisition timeout in milliseconds.
	* @param[in] timeout_ms						acquisition timeout in milliseconds.
	*/
	void SetNextImageTimeout_ms(int timeout_ms);

	/**
	* GetNextImageTimeout_ms
	* Get internal data acquisition timeout in milliseconds.
	* @return Acquisition timeout in milliseconds.
	*/
	int GetNextImageTimeout_ms();

	/**
	* GetNextImage
	* Allows the user to retrieve the frame into xiAPIplus_Image object instance.
	* @param[in] app_image						pointer to xiAPIplus_Image instance to hold data from camera.
	* @return Pointer to xiAPIplus_Image object with latest data from camera.
	*/
	xiAPIplus_Image* GetNextImage(xiAPIplus_Image* app_image);

	/**
	* GetLastImage
	* Allows the user to retrieve the pointer to internal camera xiAPIplus_Image object holding the last acquired image.
	* @return Pointer to internal camera xiAPIplus_Image object holding the last acquired image.
	*/
	xiAPIplus_Image* GetLastImage();

	/**
	* GetCameraHandle
	* Retrieve the internal camera handle used by xiApi.
	* @return Internal camera handle.
	*/
	HANDLE GetCameraHandle() { return camera_handle; }

	/**
	* SetCameraHandle
	* Change the internal camera handle used by xiApi.
	* @param[in] handle							New camera handle to be used.
	*/
	void SetCameraHandle(HANDLE handle) { camera_handle = handle; }

	// parameter access
	void GetXIAPIParamString(char* xiapi_param_name, char* value, int value_max_size);
	void SetXIAPIParamString(char* xiapi_param_name, char* value, unsigned int value_size);

	void GetXIAPIParamInt(char* xiapi_param_name, int* value);
	void SetXIAPIParamInt(char* xiapi_param_name, int value);

	void GetXIAPIParamFloat(char* xiapi_param_name, float* value);
	void SetXIAPIParamFloat(char* xiapi_param_name, float value);

	void GetXIAPIParam(char* xiapi_param_name, void* value, size_t * value_size, XI_PRM_TYPE * type);
	void SetXIAPIParam(char* xiapi_param_name, void* value, size_t value_size, XI_PRM_TYPE type);


	//-------------------------------------------------------------------------------------------------------------------
	// xiApi parameters


	//-------------------------------------------------------------------------------------------------------------------
	// ---- Parameter Group: Basic
	//-------------------------------------------------------------------------------------------------------------------

	 
	// Exposure time in microseconds (XI_PRM_EXPOSURE)
	
	float GetExposureTime();
	float GetExposureTime_Maximum();
	float GetExposureTime_Minimum();
	float GetExposureTime_Increment();
	
	void SetExposureTime(float ExposureTime);
	 
	// Selector for Exposure parameter (XI_PRM_EXPOSURE_TIME_SELECTOR)
	XI_EXPOSURE_TIME_SELECTOR_TYPE GetExposureTimeSelector();
	XI_EXPOSURE_TIME_SELECTOR_TYPE GetExposureTimeSelector_Maximum();
	XI_EXPOSURE_TIME_SELECTOR_TYPE GetExposureTimeSelector_Minimum();
	int GetExposureTimeSelector_Increment();
	
	void SetExposureTimeSelector(XI_EXPOSURE_TIME_SELECTOR_TYPE ExposureTimeSelector);
	 
	// Sets the number of times of exposure in one frame. (XI_PRM_EXPOSURE_BURST_COUNT)
	int GetExposureBurstCount();
	int GetExposureBurstCount_Maximum();
	int GetExposureBurstCount_Minimum();
	int GetExposureBurstCount_Increment();
	void SetExposureBurstCount(int ExposureBurstCount);
	 
	// Gain selector for parameter Gain allows to select different type of gains. (XI_PRM_GAIN_SELECTOR)
	XI_GAIN_SELECTOR_TYPE GetGainSelector();
	XI_GAIN_SELECTOR_TYPE GetGainSelector_Maximum();
	XI_GAIN_SELECTOR_TYPE GetGainSelector_Minimum();
	int GetGainSelector_Increment();
	
	void SetGainSelector(XI_GAIN_SELECTOR_TYPE GainSelector);
	 
	// Gain in dB (XI_PRM_GAIN)
	
	float GetGain();
	float GetGain_Maximum();
	float GetGain_Minimum();
	float GetGain_Increment();
	
	void SetGain(float Gain);
	 
	// Change image resolution by binning or skipping. (XI_PRM_DOWNSAMPLING)
	XI_DOWNSAMPLING_VALUE GetDownsampling();
	XI_DOWNSAMPLING_VALUE GetDownsampling_Maximum();
	XI_DOWNSAMPLING_VALUE GetDownsampling_Minimum();
	int GetDownsampling_Increment();
	
	void SetDownsampling(XI_DOWNSAMPLING_VALUE Downsampling);
	 
	// Change image downsampling type. (XI_PRM_DOWNSAMPLING_TYPE)
	XI_DOWNSAMPLING_TYPE GetDownsamplingType();
	XI_DOWNSAMPLING_TYPE GetDownsamplingType_Maximum();
	XI_DOWNSAMPLING_TYPE GetDownsamplingType_Minimum();
	int GetDownsamplingType_Increment();
	
	void SetDownsamplingType(XI_DOWNSAMPLING_TYPE DownsamplingType);
	 
	// Selects which test pattern generator is controlled by the test pattern feature. (XI_PRM_TEST_PATTERN_GENERATOR_SELECTOR)
	XI_TEST_PATTERN_GENERATOR GetTestPatternGeneratorSelector();
	XI_TEST_PATTERN_GENERATOR GetTestPatternGeneratorSelector_Maximum();
	XI_TEST_PATTERN_GENERATOR GetTestPatternGeneratorSelector_Minimum();
	int GetTestPatternGeneratorSelector_Increment();
	
	void SetTestPatternGeneratorSelector(XI_TEST_PATTERN_GENERATOR TestPatternGeneratorSelector);
	 
	// Selects which test pattern type is generated by the selected generator. (XI_PRM_TEST_PATTERN)
	XI_TEST_PATTERN GetTestPattern();
	XI_TEST_PATTERN GetTestPattern_Maximum();
	XI_TEST_PATTERN GetTestPattern_Minimum();
	int GetTestPattern_Increment();
	
	void SetTestPattern(XI_TEST_PATTERN TestPattern);
	 
	// Output data format. (XI_PRM_IMAGE_DATA_FORMAT)
	XI_IMG_FORMAT GetImageDataFormat();
	XI_IMG_FORMAT GetImageDataFormat_Maximum();
	XI_IMG_FORMAT GetImageDataFormat_Minimum();
	int GetImageDataFormat_Increment();
	
	void SetImageDataFormat(XI_IMG_FORMAT ImageDataFormat);
	 
	// Signedness of image data. (XI_PRM_IMAGE_DATA_SIGN)
	XI_DATA_SM GetImageDataSign();
	XI_DATA_SM GetImageDataSign_Maximum();
	XI_DATA_SM GetImageDataSign_Minimum();
	int GetImageDataSign_Increment();
	 
	// Change sensor shutter type(CMOS sensor). (XI_PRM_SHUTTER_TYPE)
	XI_SHUTTER_TYPE GetShutterType();
	XI_SHUTTER_TYPE GetShutterType_Maximum();
	XI_SHUTTER_TYPE GetShutterType_Minimum();
	int GetShutterType_Increment();
	
	void SetShutterType(XI_SHUTTER_TYPE ShutterType);
	 
	// Number of taps (XI_PRM_SENSOR_TAPS)
	XI_SENSOR_TAP_CNT GetSensorTaps();
	XI_SENSOR_TAP_CNT GetSensorTaps_Maximum();
	XI_SENSOR_TAP_CNT GetSensorTaps_Minimum();
	int GetSensorTaps_Increment();
	
	void SetSensorTaps(XI_SENSOR_TAP_CNT SensorTaps);
	 
	// Automatic exposure/gain (XI_PRM_AEAG)
	
	bool IsAutoExposureAutoGain();
	
	void EnableAutoExposureAutoGain();
	void DisableAutoExposureAutoGain();
	 
	// Automatic exposure/gain ROI offset X (XI_PRM_AEAG_ROI_OFFSET_X)
	int GetAutoExposureAutoGainROIoffsetX();
	int GetAutoExposureAutoGainROIoffsetX_Maximum();
	int GetAutoExposureAutoGainROIoffsetX_Minimum();
	int GetAutoExposureAutoGainROIoffsetX_Increment();
	void SetAutoExposureAutoGainROIoffsetX(int AutoExposureAutoGainROIoffsetX);
	 
	// Automatic exposure/gain ROI offset Y (XI_PRM_AEAG_ROI_OFFSET_Y)
	int GetAutoExposureAutoGainROIoffsetY();
	int GetAutoExposureAutoGainROIoffsetY_Maximum();
	int GetAutoExposureAutoGainROIoffsetY_Minimum();
	int GetAutoExposureAutoGainROIoffsetY_Increment();
	void SetAutoExposureAutoGainROIoffsetY(int AutoExposureAutoGainROIoffsetY);
	 
	// Automatic exposure/gain ROI Width (XI_PRM_AEAG_ROI_WIDTH)
	int GetAutoExposureAutoGainROIWidth();
	int GetAutoExposureAutoGainROIWidth_Maximum();
	int GetAutoExposureAutoGainROIWidth_Minimum();
	int GetAutoExposureAutoGainROIWidth_Increment();
	void SetAutoExposureAutoGainROIWidth(int AutoExposureAutoGainROIWidth);
	 
	// Automatic exposure/gain ROI Height (XI_PRM_AEAG_ROI_HEIGHT)
	int GetAutoExposureAutoGainROIHeight();
	int GetAutoExposureAutoGainROIHeight_Maximum();
	int GetAutoExposureAutoGainROIHeight_Minimum();
	int GetAutoExposureAutoGainROIHeight_Increment();
	void SetAutoExposureAutoGainROIHeight(int AutoExposureAutoGainROIHeight);
	 
	// Selector of list used by Sensor Defects Correction parameter (XI_PRM_SENS_DEFECTS_CORR_LIST_SELECTOR)
	XI_SENS_DEFFECTS_CORR_LIST_SELECTOR GetSensorDefectsCorrectionListSelector();
	XI_SENS_DEFFECTS_CORR_LIST_SELECTOR GetSensorDefectsCorrectionListSelector_Maximum();
	XI_SENS_DEFFECTS_CORR_LIST_SELECTOR GetSensorDefectsCorrectionListSelector_Minimum();
	int GetSensorDefectsCorrectionListSelector_Increment();
	
	void SetSensorDefectsCorrectionListSelector(XI_SENS_DEFFECTS_CORR_LIST_SELECTOR SensorDefectsCorrectionListSelector);
	 
	// Sets/Gets sensor defects list in special text format (XI_PRM_SENS_DEFECTS_CORR_LIST_CONTENT)
	
	void GetSensDefectsCorrListContent(char* buffer, int buffer_lenght);
	
	void SetSensDefectsCorrListContent(char* SensDefectsCorrListContent);
	 
	// Correction of sensor defects (pixels, columns, rows) enable/disable (XI_PRM_SENS_DEFECTS_CORR)
	
	bool IsSensorDefectsCorrection();
	
	void EnableSensorDefectsCorrection();
	void DisableSensorDefectsCorrection();
	 
	// Automatic white balance (XI_PRM_AUTO_WB)
	
	bool IsWhiteBalanceAuto();
	
	void EnableWhiteBalanceAuto();
	void DisableWhiteBalanceAuto();
	 
	// Calculates White Balance(xiGetImage function must be called) (XI_PRM_MANUAL_WB)
	
	void SetWhiteBalanceManualNow(int WhiteBalanceManualNow);
	 
	// White balance offset X (XI_PRM_WB_ROI_OFFSET_X)
	int GetWhiteBalanceROIoffsetX();
	int GetWhiteBalanceROIoffsetX_Maximum();
	int GetWhiteBalanceROIoffsetX_Minimum();
	int GetWhiteBalanceROIoffsetX_Increment();
	void SetWhiteBalanceROIoffsetX(int WhiteBalanceROIoffsetX);
	 
	// White balance offset Y (XI_PRM_WB_ROI_OFFSET_Y)
	int GetWhiteBalanceROIoffsetY();
	int GetWhiteBalanceROIoffsetY_Maximum();
	int GetWhiteBalanceROIoffsetY_Minimum();
	int GetWhiteBalanceROIoffsetY_Increment();
	void SetWhiteBalanceROIoffsetY(int WhiteBalanceROIoffsetY);
	 
	// White balance width (XI_PRM_WB_ROI_WIDTH)
	int GetWhiteBalanceROIWidth();
	int GetWhiteBalanceROIWidth_Maximum();
	int GetWhiteBalanceROIWidth_Minimum();
	int GetWhiteBalanceROIWidth_Increment();
	void SetWhiteBalanceROIWidth(int WhiteBalanceROIWidth);
	 
	// White balance height (XI_PRM_WB_ROI_HEIGHT)
	int GetWhiteBalanceROIHeight();
	int GetWhiteBalanceROIHeight_Maximum();
	int GetWhiteBalanceROIHeight_Minimum();
	int GetWhiteBalanceROIHeight_Increment();
	void SetWhiteBalanceROIHeight(int WhiteBalanceROIHeight);
	 
	// White balance red coefficient (XI_PRM_WB_KR)
	
	float GetWhiteBalanceRed();
	float GetWhiteBalanceRed_Maximum();
	float GetWhiteBalanceRed_Minimum();
	float GetWhiteBalanceRed_Increment();
	
	void SetWhiteBalanceRed(float WhiteBalanceRed);
	 
	// White balance green coefficient (XI_PRM_WB_KG)
	
	float GetWhiteBalanceGreen();
	float GetWhiteBalanceGreen_Maximum();
	float GetWhiteBalanceGreen_Minimum();
	float GetWhiteBalanceGreen_Increment();
	
	void SetWhiteBalanceGreen(float WhiteBalanceGreen);
	 
	// White balance blue coefficient (XI_PRM_WB_KB)
	
	float GetWhiteBalanceBlue();
	float GetWhiteBalanceBlue_Maximum();
	float GetWhiteBalanceBlue_Minimum();
	float GetWhiteBalanceBlue_Increment();
	
	void SetWhiteBalanceBlue(float WhiteBalanceBlue);
	 
	// Width of the Image provided by the device (in pixels). (XI_PRM_WIDTH)
	int GetWidth();
	int GetWidth_Maximum();
	int GetWidth_Minimum();
	int GetWidth_Increment();
	void SetWidth(int Width);
	 
	// Height of the Image provided by the device (in pixels). (XI_PRM_HEIGHT)
	int GetHeight();
	int GetHeight_Maximum();
	int GetHeight_Minimum();
	int GetHeight_Increment();
	void SetHeight(int Height);
	 
	// Horizontal offset from the origin to the area of interest (in pixels). (XI_PRM_OFFSET_X)
	int GetOffsetX();
	int GetOffsetX_Maximum();
	int GetOffsetX_Minimum();
	int GetOffsetX_Increment();
	void SetOffsetX(int OffsetX);
	 
	// Vertical offset from the origin to the area of interest (in pixels). (XI_PRM_OFFSET_Y)
	int GetOffsetY();
	int GetOffsetY_Maximum();
	int GetOffsetY_Minimum();
	int GetOffsetY_Increment();
	void SetOffsetY(int OffsetY);
	 
	// Selects Region in Multiple ROI which parameters are set by width, height, ... ,region mode (XI_PRM_REGION_SELECTOR)
	int GetRegionSelector();
	int GetRegionSelector_Maximum();
	int GetRegionSelector_Minimum();
	int GetRegionSelector_Increment();
	void SetRegionSelector(int RegionSelector);
	 
	// Activates/deactivates Region selected by Region Selector (XI_PRM_REGION_MODE)
	int GetRegionMode();
	int GetRegionMode_Maximum();
	int GetRegionMode_Minimum();
	int GetRegionMode_Increment();
	void SetRegionMode(int RegionMode);
	 
	// Horizontal flip enable (XI_PRM_HORIZONTAL_FLIP)
	
	bool IsHorizontalFlip();
	
	void EnableHorizontalFlip();
	void DisableHorizontalFlip();
	 
	// Vertical flip enable (XI_PRM_VERTICAL_FLIP)
	
	bool IsVerticalFlip();
	
	void EnableVerticalFlip();
	void DisableVerticalFlip();
	 
	// Selector for Exposure parameter (XI_PRM_INTERLINE_EXPOSURE_MODE)
	XI_INTERLINE_EXPOSURE_MODE_TYPE GetInterlineExposureMode();
	XI_INTERLINE_EXPOSURE_MODE_TYPE GetInterlineExposureMode_Maximum();
	XI_INTERLINE_EXPOSURE_MODE_TYPE GetInterlineExposureMode_Minimum();
	int GetInterlineExposureMode_Increment();
	
	void SetInterlineExposureMode(XI_INTERLINE_EXPOSURE_MODE_TYPE InterlineExposureMode);
	 
	// Image flat field correction (XI_PRM_FFC)
	
	bool IsFlatFieldCorrection();
	
	void EnableFlatFieldCorrection();
	void DisableFlatFieldCorrection();
	 
	// Set name of file to be applied for FFC processor. (XI_PRM_FFC_FLAT_FIELD_FILE_NAME)
	
	void GetFFCFlatFieldFileName(char* buffer, int buffer_lenght);
	
	void SetFFCFlatFieldFileName(char* FFCFlatFieldFileName);
	 
	// Set name of file to be applied for FFC processor. (XI_PRM_FFC_DARK_FIELD_FILE_NAME)
	
	void GetFFCDarkFieldFileName(char* buffer, int buffer_lenght);
	
	void SetFFCDarkFieldFileName(char* FFCDarkFieldFileName);
	 
	// Sets ToF Readout Mode (XI_PRM_TOF_READOUT_MODE)
	XI_TOF_READOUT_MODE GetTofReadoutMode();
	XI_TOF_READOUT_MODE GetTofReadoutMode_Maximum();
	XI_TOF_READOUT_MODE GetTofReadoutMode_Minimum();
	int GetTofReadoutMode_Increment();
	
	void SetTofReadoutMode(XI_TOF_READOUT_MODE TofReadoutMode);
	 
	// Sets ToF Modulation Frequency in MHz (XI_PRM_TOF_MODULATION_FREQUENCY)
	
	float GetTofModulationFrequency();
	float GetTofModulationFrequency_Maximum();
	float GetTofModulationFrequency_Minimum();
	float GetTofModulationFrequency_Increment();
	
	void SetTofModulationFrequency(float TofModulationFrequency);
	 
	// is multiple ToF phases concatenated in buffer (XI_PRM_TOF_MULTIPLE_PHASES_IN_BUFFER)
	int GetTofMultiplePhasesInBuffer();
	int GetTofMultiplePhasesInBuffer_Maximum();
	int GetTofMultiplePhasesInBuffer_Minimum();
	int GetTofMultiplePhasesInBuffer_Increment(); 
	// Sets the number of tof phases. E.g. 4 for four phases. (XI_PRM_TOF_PHASES_COUNT)
	int GetTofPhasesCount();
	int GetTofPhasesCount_Maximum();
	int GetTofPhasesCount_Minimum();
	int GetTofPhasesCount_Increment();
	void SetTofPhasesCount(int TofPhasesCount);
	 
	// Sets Illumination angle for selected ToF phase (XI_PRM_TOF_PHASE_ANGLE)
	
	float GetTofPhaseAngle();
	float GetTofPhaseAngle_Maximum();
	float GetTofPhaseAngle_Minimum();
	float GetTofPhaseAngle_Increment();
	
	void SetTofPhaseAngle(float TofPhaseAngle);
	 
	// Sets Exposure time for selected ToF phase in microseconds. (XI_PRM_TOF_PHASE_EXPOSURE_TIME)
	
	float GetTofPhaseExposureTime();
	float GetTofPhaseExposureTime_Maximum();
	float GetTofPhaseExposureTime_Minimum();
	float GetTofPhaseExposureTime_Increment();
	
	void SetTofPhaseExposureTime(float TofPhaseExposureTime);
	 
	// Selects tof phase (XI_PRM_TOF_PHASE_SELECTOR)
	int GetTofPhaseSelector();
	int GetTofPhaseSelector_Maximum();
	int GetTofPhaseSelector_Minimum();
	int GetTofPhaseSelector_Increment();
	void SetTofPhaseSelector(int TofPhaseSelector);
	

	//-------------------------------------------------------------------------------------------------------------------
	// ---- Parameter Group: Image Format
	//-------------------------------------------------------------------------------------------------------------------

	 
	// Binning engine selector. (XI_PRM_BINNING_SELECTOR)
	XI_BIN_SELECTOR GetBinningSelector();
	XI_BIN_SELECTOR GetBinningSelector_Maximum();
	XI_BIN_SELECTOR GetBinningSelector_Minimum();
	int GetBinningSelector_Increment();
	
	void SetBinningSelector(XI_BIN_SELECTOR BinningSelector);
	 
	// Sets the mode to use to combine vertical pixel together. (XI_PRM_BINNING_VERTICAL_MODE)
	XI_BIN_MODE GetBinningVerticalMode();
	XI_BIN_MODE GetBinningVerticalMode_Maximum();
	XI_BIN_MODE GetBinningVerticalMode_Minimum();
	int GetBinningVerticalMode_Increment();
	
	void SetBinningVerticalMode(XI_BIN_MODE BinningVerticalMode);
	 
	// Vertical Binning - number of vertical photo-sensitive cells to combine together. (XI_PRM_BINNING_VERTICAL)
	int GetBinningVertical();
	int GetBinningVertical_Maximum();
	int GetBinningVertical_Minimum();
	int GetBinningVertical_Increment();
	void SetBinningVertical(int BinningVertical);
	 
	// Vertical Binning Float - number of vertical photo-sensitive cells to combine together. (XI_PRM_BINNING_VERTICAL_FLOAT)
	
	float GetBinningVerticalFloat();
	float GetBinningVerticalFloat_Maximum();
	float GetBinningVerticalFloat_Minimum();
	float GetBinningVerticalFloat_Increment();
	
	void SetBinningVerticalFloat(float BinningVerticalFloat);
	 
	// Sets the mode to use to combine horizontal pixel together. (XI_PRM_BINNING_HORIZONTAL_MODE)
	XI_BIN_MODE GetBinningHorizontalMode();
	XI_BIN_MODE GetBinningHorizontalMode_Maximum();
	XI_BIN_MODE GetBinningHorizontalMode_Minimum();
	int GetBinningHorizontalMode_Increment();
	
	void SetBinningHorizontalMode(XI_BIN_MODE BinningHorizontalMode);
	 
	// Horizontal Binning - number of horizontal photo-sensitive cells to combine together. (XI_PRM_BINNING_HORIZONTAL)
	int GetBinningHorizontal();
	int GetBinningHorizontal_Maximum();
	int GetBinningHorizontal_Minimum();
	int GetBinningHorizontal_Increment();
	void SetBinningHorizontal(int BinningHorizontal);
	 
	// Horizontal Binning Float - number of horizontal photo-sensitive cells to combine together. (XI_PRM_BINNING_HORIZONTAL_FLOAT)
	
	float GetBinningHorizontalFloat();
	float GetBinningHorizontalFloat_Maximum();
	float GetBinningHorizontalFloat_Minimum();
	float GetBinningHorizontalFloat_Increment();
	
	void SetBinningHorizontalFloat(float BinningHorizontalFloat);
	 
	// Binning horizontal pattern type. (XI_PRM_BINNING_HORIZONTAL_PATTERN)
	XI_BIN_PATTERN GetBinningHorizontalPattern();
	XI_BIN_PATTERN GetBinningHorizontalPattern_Maximum();
	XI_BIN_PATTERN GetBinningHorizontalPattern_Minimum();
	int GetBinningHorizontalPattern_Increment();
	
	void SetBinningHorizontalPattern(XI_BIN_PATTERN BinningHorizontalPattern);
	 
	// Binning vertical pattern type. (XI_PRM_BINNING_VERTICAL_PATTERN)
	XI_BIN_PATTERN GetBinningVerticalPattern();
	XI_BIN_PATTERN GetBinningVerticalPattern_Maximum();
	XI_BIN_PATTERN GetBinningVerticalPattern_Minimum();
	int GetBinningVerticalPattern_Increment();
	
	void SetBinningVerticalPattern(XI_BIN_PATTERN BinningVerticalPattern);
	 
	// Decimation engine selector. (XI_PRM_DECIMATION_SELECTOR)
	XI_DEC_SELECTOR GetDecimationSelector();
	XI_DEC_SELECTOR GetDecimationSelector_Maximum();
	XI_DEC_SELECTOR GetDecimationSelector_Minimum();
	int GetDecimationSelector_Increment();
	
	void SetDecimationSelector(XI_DEC_SELECTOR DecimationSelector);
	 
	// Vertical Decimation - vertical sub-sampling of the image - reduces the vertical resolution of the image by the specified vertical decimation factor. (XI_PRM_DECIMATION_VERTICAL)
	int GetDecimationVertical();
	int GetDecimationVertical_Maximum();
	int GetDecimationVertical_Minimum();
	int GetDecimationVertical_Increment();
	void SetDecimationVertical(int DecimationVertical);
	 
	// Horizontal Decimation - horizontal sub-sampling of the image - reduces the horizontal resolution of the image by the specified vertical decimation factor. (XI_PRM_DECIMATION_HORIZONTAL)
	int GetDecimationHorizontal();
	int GetDecimationHorizontal_Maximum();
	int GetDecimationHorizontal_Minimum();
	int GetDecimationHorizontal_Increment();
	void SetDecimationHorizontal(int DecimationHorizontal);
	 
	// Decimation horizontal pattern type. (XI_PRM_DECIMATION_HORIZONTAL_PATTERN)
	XI_DEC_PATTERN GetDecimationHorizontalPattern();
	XI_DEC_PATTERN GetDecimationHorizontalPattern_Maximum();
	XI_DEC_PATTERN GetDecimationHorizontalPattern_Minimum();
	int GetDecimationHorizontalPattern_Increment();
	
	void SetDecimationHorizontalPattern(XI_DEC_PATTERN DecimationHorizontalPattern);
	 
	// Decimation vertical pattern type. (XI_PRM_DECIMATION_VERTICAL_PATTERN)
	XI_DEC_PATTERN GetDecimationVerticalPattern();
	XI_DEC_PATTERN GetDecimationVerticalPattern_Maximum();
	XI_DEC_PATTERN GetDecimationVerticalPattern_Minimum();
	int GetDecimationVerticalPattern_Increment();
	
	void SetDecimationVerticalPattern(XI_DEC_PATTERN DecimationVerticalPattern);
	

	//-------------------------------------------------------------------------------------------------------------------
	// ---- Parameter Group: AE Setup
	//-------------------------------------------------------------------------------------------------------------------

	 
	// Exposure priority (0.8 - exposure 80%, gain 20%). (XI_PRM_EXP_PRIORITY)
	
	float GetAutoExposureAutoGainExposurePriority();
	float GetAutoExposureAutoGainExposurePriority_Maximum();
	float GetAutoExposureAutoGainExposurePriority_Minimum();
	float GetAutoExposureAutoGainExposurePriority_Increment();
	
	void SetAutoExposureAutoGainExposurePriority(float AutoExposureAutoGainExposurePriority);
	 
	// Maximum limit of gain in AEAG procedure (XI_PRM_AG_MAX_LIMIT)
	
	float GetAutoGainTopLimit();
	float GetAutoGainTopLimit_Maximum();
	float GetAutoGainTopLimit_Minimum();
	float GetAutoGainTopLimit_Increment();
	
	void SetAutoGainTopLimit(float AutoGainTopLimit);
	 
	// Maximum time (us) used for exposure in AEAG procedure (XI_PRM_AE_MAX_LIMIT)
	int GetAutoExposureTopLimit();
	int GetAutoExposureTopLimit_Maximum();
	int GetAutoExposureTopLimit_Minimum();
	int GetAutoExposureTopLimit_Increment();
	void SetAutoExposureTopLimit(int AutoExposureTopLimit);
	 
	// Average intensity of output signal AEAG should achieve(in %) (XI_PRM_AEAG_LEVEL)
	int GetAutoExposureAutoGainTargetLevel();
	int GetAutoExposureAutoGainTargetLevel_Maximum();
	int GetAutoExposureAutoGainTargetLevel_Minimum();
	int GetAutoExposureAutoGainTargetLevel_Increment();
	void SetAutoExposureAutoGainTargetLevel(int AutoExposureAutoGainTargetLevel);
	 
	// Minimum number of frames to be skipped between AEAG compensation and AEAG re-evaluation (XI_PRM_AEAG_SKIP_FRAMES_COUNT)
	int GetAutoExposureAutoGainSkipFramesCount();
	int GetAutoExposureAutoGainSkipFramesCount_Maximum();
	int GetAutoExposureAutoGainSkipFramesCount_Minimum();
	int GetAutoExposureAutoGainSkipFramesCount_Increment();
	void SetAutoExposureAutoGainSkipFramesCount(int AutoExposureAutoGainSkipFramesCount);
	

	//-------------------------------------------------------------------------------------------------------------------
	// ---- Parameter Group: Performance
	//-------------------------------------------------------------------------------------------------------------------

	 
	// Set/get bandwidth(data rate in Megabits) (XI_PRM_LIMIT_BANDWIDTH)
	int GetBandwidthLimit();
	int GetBandwidthLimit_Maximum();
	int GetBandwidthLimit_Minimum();
	int GetBandwidthLimit_Increment();
	void SetBandwidthLimit(int BandwidthLimit);
	 
	// Bandwidth limit enabled (XI_PRM_LIMIT_BANDWIDTH_MODE)
	XI_SWITCH GetBandwidthLimitMode();
	XI_SWITCH GetBandwidthLimitMode_Maximum();
	XI_SWITCH GetBandwidthLimitMode_Minimum();
	int GetBandwidthLimitMode_Increment();
	
	void SetBandwidthLimitMode(XI_SWITCH BandwidthLimitMode);
	 
	// Sensor output data bit depth. (XI_PRM_SENSOR_DATA_BIT_DEPTH)
	XI_BIT_DEPTH GetSensorDataBitDepth();
	XI_BIT_DEPTH GetSensorDataBitDepth_Maximum();
	XI_BIT_DEPTH GetSensorDataBitDepth_Minimum();
	int GetSensorDataBitDepth_Increment();
	
	void SetSensorDataBitDepth(XI_BIT_DEPTH SensorDataBitDepth);
	 
	// Device output data bit depth. (XI_PRM_OUTPUT_DATA_BIT_DEPTH)
	XI_BIT_DEPTH GetDeviceOutputDataBitDepth();
	XI_BIT_DEPTH GetDeviceOutputDataBitDepth_Maximum();
	XI_BIT_DEPTH GetDeviceOutputDataBitDepth_Minimum();
	int GetDeviceOutputDataBitDepth_Increment();
	
	void SetDeviceOutputDataBitDepth(XI_BIT_DEPTH DeviceOutputDataBitDepth);
	 
	// bit depth of data returned by function xiGetImage (XI_PRM_IMAGE_DATA_BIT_DEPTH)
	XI_BIT_DEPTH GetImageDataBitDepth();
	XI_BIT_DEPTH GetImageDataBitDepth_Maximum();
	XI_BIT_DEPTH GetImageDataBitDepth_Minimum();
	int GetImageDataBitDepth_Increment();
	
	void SetImageDataBitDepth(XI_BIT_DEPTH ImageDataBitDepth);
	 
	// Device output data packing (or grouping) enabled. Packing could be enabled if output_data_bit_depth > 8 and packing capability is available. (XI_PRM_OUTPUT_DATA_PACKING)
	
	bool IsDeviceOutputDataPacking();
	
	void EnableDeviceOutputDataPacking();
	void DisableDeviceOutputDataPacking();
	 
	// Data packing type. Some cameras supports only specific packing type. (XI_PRM_OUTPUT_DATA_PACKING_TYPE)
	XI_OUTPUT_DATA_PACKING_TYPE GetDeviceOutputDataPackingType();
	XI_OUTPUT_DATA_PACKING_TYPE GetDeviceOutputDataPackingType_Maximum();
	XI_OUTPUT_DATA_PACKING_TYPE GetDeviceOutputDataPackingType_Minimum();
	int GetDeviceOutputDataPackingType_Increment();
	
	void SetDeviceOutputDataPackingType(XI_OUTPUT_DATA_PACKING_TYPE DeviceOutputDataPackingType);
	

	//-------------------------------------------------------------------------------------------------------------------
	// ---- Parameter Group: Temperature
	//-------------------------------------------------------------------------------------------------------------------

	 
	// Returns 1 for cameras that support cooling. (XI_PRM_IS_COOLED)
	
	bool IsCooled();
	 
	// Temperature control mode. (XI_PRM_COOLING)
	XI_TEMP_CTRL_MODE_SELECTOR GetCooling();
	XI_TEMP_CTRL_MODE_SELECTOR GetCooling_Maximum();
	XI_TEMP_CTRL_MODE_SELECTOR GetCooling_Minimum();
	int GetCooling_Increment();
	
	void SetCooling(XI_TEMP_CTRL_MODE_SELECTOR Cooling);
	 
	// Set sensor target temperature for cooling. (XI_PRM_TARGET_TEMP)
	
	float GetTargetTemperature();
	float GetTargetTemperature_Maximum();
	float GetTargetTemperature_Minimum();
	float GetTargetTemperature_Increment();
	
	void SetTargetTemperature(float TargetTemperature);
	 
	// Selector of mechanical point where thermometer is located. (XI_PRM_TEMP_SELECTOR)
	XI_TEMP_SELECTOR GetTemperatureSelector();
	XI_TEMP_SELECTOR GetTemperatureSelector_Maximum();
	XI_TEMP_SELECTOR GetTemperatureSelector_Minimum();
	int GetTemperatureSelector_Increment();
	
	void SetTemperatureSelector(XI_TEMP_SELECTOR TemperatureSelector);
	 
	// Camera temperature (selected by XI_PRM_TEMP_SELECTOR) (XI_PRM_TEMP)
	
	float GetTemperature();
	float GetTemperature_Maximum();
	float GetTemperature_Minimum();
	float GetTemperature_Increment();
	 
	// Temperature control mode. (XI_PRM_TEMP_CONTROL_MODE)
	XI_TEMP_CTRL_MODE_SELECTOR GetTemperatureControlMode();
	XI_TEMP_CTRL_MODE_SELECTOR GetTemperatureControlMode_Maximum();
	XI_TEMP_CTRL_MODE_SELECTOR GetTemperatureControlMode_Minimum();
	int GetTemperatureControlMode_Increment();
	
	void SetTemperatureControlMode(XI_TEMP_CTRL_MODE_SELECTOR TemperatureControlMode);
	 
	// Camera sensor temperature (XI_PRM_CHIP_TEMP)
	
	float GetTemperatureSensor();
	float GetTemperatureSensor_Maximum();
	float GetTemperatureSensor_Minimum();
	float GetTemperatureSensor_Increment();
	 
	// Camera housing temperature (XI_PRM_HOUS_TEMP)
	
	float GetTemperatureHousing();
	float GetTemperatureHousing_Maximum();
	float GetTemperatureHousing_Minimum();
	float GetTemperatureHousing_Increment();
	 
	// Camera housing back side temperature (XI_PRM_HOUS_BACK_SIDE_TEMP)
	
	float GetTemperatureHousingBackSide();
	float GetTemperatureHousingBackSide_Maximum();
	float GetTemperatureHousingBackSide_Minimum();
	float GetTemperatureHousingBackSide_Increment();
	 
	// Camera sensor board temperature (XI_PRM_SENSOR_BOARD_TEMP)
	
	float GetTemperatureSensorBoard();
	float GetTemperatureSensorBoard_Maximum();
	float GetTemperatureSensorBoard_Minimum();
	float GetTemperatureSensorBoard_Increment();
	 
	// Temperature element selector (TEC(Peltier), Fan). (XI_PRM_TEMP_ELEMENT_SEL)
	XI_TEMP_ELEMENT_SELECTOR GetTemperatureElementSelector();
	XI_TEMP_ELEMENT_SELECTOR GetTemperatureElementSelector_Maximum();
	XI_TEMP_ELEMENT_SELECTOR GetTemperatureElementSelector_Minimum();
	int GetTemperatureElementSelector_Increment();
	
	void SetTemperatureElementSelector(XI_TEMP_ELEMENT_SELECTOR TemperatureElementSelector);
	 
	// Temperature element value in percents of full control range (XI_PRM_TEMP_ELEMENT_VALUE)
	
	float GetTemperatureElementValue();
	float GetTemperatureElementValue_Maximum();
	float GetTemperatureElementValue_Minimum();
	float GetTemperatureElementValue_Increment();
	
	void SetTemperatureElementValue(float TemperatureElementValue);
	

	//-------------------------------------------------------------------------------------------------------------------
	// ---- Parameter Group: Color Correction
	//-------------------------------------------------------------------------------------------------------------------

	 
	// Mode of color management system. (XI_PRM_CMS)
	XI_CMS_MODE GetColorManagementMode();
	XI_CMS_MODE GetColorManagementMode_Maximum();
	XI_CMS_MODE GetColorManagementMode_Minimum();
	int GetColorManagementMode_Increment();
	
	void SetColorManagementMode(XI_CMS_MODE ColorManagementMode);
	 
	// Intent of color management system. (XI_PRM_CMS_INTENT)
	XI_CMS_INTENT GetColorManagementIntent();
	XI_CMS_INTENT GetColorManagementIntent_Maximum();
	XI_CMS_INTENT GetColorManagementIntent_Minimum();
	int GetColorManagementIntent_Increment();
	
	void SetColorManagementIntent(XI_CMS_INTENT ColorManagementIntent);
	 
	// Enable applying of CMS profiles to xiGetImage (see XI_PRM_INPUT_CMS_PROFILE, XI_PRM_OUTPUT_CMS_PROFILE). (XI_PRM_APPLY_CMS)
	
	bool IsColorManagementApply();
	
	void EnableColorManagementApply();
	void DisableColorManagementApply();
	 
	// Filename for input cms profile (e.g. input.icc) (XI_PRM_INPUT_CMS_PROFILE)
	
	void GetColorManagementInputProfile(char* buffer, int buffer_lenght);
	
	void SetColorManagementInputProfile(char* ColorManagementInputProfile);
	 
	// Filename for output cms profile (e.g. input.icc) (XI_PRM_OUTPUT_CMS_PROFILE)
	
	void GetColorManagementOutputProfile(char* buffer, int buffer_lenght);
	
	void SetColorManagementOutputProfile(char* ColorManagementOutputProfile);
	 
	// Returns 1 for color cameras. (XI_PRM_IMAGE_IS_COLOR)
	
	bool IsSensorColor();
	 
	// Returns color filter array type of RAW data. (XI_PRM_COLOR_FILTER_ARRAY)
	XI_COLOR_FILTER_ARRAY GetSensorColorFilterArray();
	XI_COLOR_FILTER_ARRAY GetSensorColorFilterArray_Maximum();
	XI_COLOR_FILTER_ARRAY GetSensorColorFilterArray_Minimum();
	int GetSensorColorFilterArray_Increment();
	 
	// Luminosity gamma (XI_PRM_GAMMAY)
	
	float GetGammaLuminosity();
	float GetGammaLuminosity_Maximum();
	float GetGammaLuminosity_Minimum();
	float GetGammaLuminosity_Increment();
	
	void SetGammaLuminosity(float GammaLuminosity);
	 
	// Chromaticity gamma (XI_PRM_GAMMAC)
	
	float GetGammaChromaticity();
	float GetGammaChromaticity_Maximum();
	float GetGammaChromaticity_Minimum();
	float GetGammaChromaticity_Increment();
	
	void SetGammaChromaticity(float GammaChromaticity);
	 
	// Sharpness strength (XI_PRM_SHARPNESS)
	
	float GetSharpness();
	float GetSharpness_Maximum();
	float GetSharpness_Minimum();
	float GetSharpness_Increment();
	
	void SetSharpness(float Sharpness);
	 
	// Color Correction Matrix element [0][0] (XI_PRM_CC_MATRIX_00)
	
	float GetColorCorrectionMatrix00();
	float GetColorCorrectionMatrix00_Maximum();
	float GetColorCorrectionMatrix00_Minimum();
	float GetColorCorrectionMatrix00_Increment();
	
	void SetColorCorrectionMatrix00(float ColorCorrectionMatrix00);
	 
	// Color Correction Matrix element [0][1] (XI_PRM_CC_MATRIX_01)
	
	float GetColorCorrectionMatrix01();
	float GetColorCorrectionMatrix01_Maximum();
	float GetColorCorrectionMatrix01_Minimum();
	float GetColorCorrectionMatrix01_Increment();
	
	void SetColorCorrectionMatrix01(float ColorCorrectionMatrix01);
	 
	// Color Correction Matrix element [0][2] (XI_PRM_CC_MATRIX_02)
	
	float GetColorCorrectionMatrix02();
	float GetColorCorrectionMatrix02_Maximum();
	float GetColorCorrectionMatrix02_Minimum();
	float GetColorCorrectionMatrix02_Increment();
	
	void SetColorCorrectionMatrix02(float ColorCorrectionMatrix02);
	 
	// Color Correction Matrix element [0][3] (XI_PRM_CC_MATRIX_03)
	
	float GetColorCorrectionMatrix03();
	float GetColorCorrectionMatrix03_Maximum();
	float GetColorCorrectionMatrix03_Minimum();
	float GetColorCorrectionMatrix03_Increment();
	
	void SetColorCorrectionMatrix03(float ColorCorrectionMatrix03);
	 
	// Color Correction Matrix element [1][0] (XI_PRM_CC_MATRIX_10)
	
	float GetColorCorrectionMatrix10();
	float GetColorCorrectionMatrix10_Maximum();
	float GetColorCorrectionMatrix10_Minimum();
	float GetColorCorrectionMatrix10_Increment();
	
	void SetColorCorrectionMatrix10(float ColorCorrectionMatrix10);
	 
	// Color Correction Matrix element [1][1] (XI_PRM_CC_MATRIX_11)
	
	float GetColorCorrectionMatrix11();
	float GetColorCorrectionMatrix11_Maximum();
	float GetColorCorrectionMatrix11_Minimum();
	float GetColorCorrectionMatrix11_Increment();
	
	void SetColorCorrectionMatrix11(float ColorCorrectionMatrix11);
	 
	// Color Correction Matrix element [1][2] (XI_PRM_CC_MATRIX_12)
	
	float GetColorCorrectionMatrix12();
	float GetColorCorrectionMatrix12_Maximum();
	float GetColorCorrectionMatrix12_Minimum();
	float GetColorCorrectionMatrix12_Increment();
	
	void SetColorCorrectionMatrix12(float ColorCorrectionMatrix12);
	 
	// Color Correction Matrix element [1][3] (XI_PRM_CC_MATRIX_13)
	
	float GetColorCorrectionMatrix13();
	float GetColorCorrectionMatrix13_Maximum();
	float GetColorCorrectionMatrix13_Minimum();
	float GetColorCorrectionMatrix13_Increment();
	
	void SetColorCorrectionMatrix13(float ColorCorrectionMatrix13);
	 
	// Color Correction Matrix element [2][0] (XI_PRM_CC_MATRIX_20)
	
	float GetColorCorrectionMatrix20();
	float GetColorCorrectionMatrix20_Maximum();
	float GetColorCorrectionMatrix20_Minimum();
	float GetColorCorrectionMatrix20_Increment();
	
	void SetColorCorrectionMatrix20(float ColorCorrectionMatrix20);
	 
	// Color Correction Matrix element [2][1] (XI_PRM_CC_MATRIX_21)
	
	float GetColorCorrectionMatrix21();
	float GetColorCorrectionMatrix21_Maximum();
	float GetColorCorrectionMatrix21_Minimum();
	float GetColorCorrectionMatrix21_Increment();
	
	void SetColorCorrectionMatrix21(float ColorCorrectionMatrix21);
	 
	// Color Correction Matrix element [2][2] (XI_PRM_CC_MATRIX_22)
	
	float GetColorCorrectionMatrix22();
	float GetColorCorrectionMatrix22_Maximum();
	float GetColorCorrectionMatrix22_Minimum();
	float GetColorCorrectionMatrix22_Increment();
	
	void SetColorCorrectionMatrix22(float ColorCorrectionMatrix22);
	 
	// Color Correction Matrix element [2][3] (XI_PRM_CC_MATRIX_23)
	
	float GetColorCorrectionMatrix23();
	float GetColorCorrectionMatrix23_Maximum();
	float GetColorCorrectionMatrix23_Minimum();
	float GetColorCorrectionMatrix23_Increment();
	
	void SetColorCorrectionMatrix23(float ColorCorrectionMatrix23);
	 
	// Color Correction Matrix element [3][0] (XI_PRM_CC_MATRIX_30)
	
	float GetColorCorrectionMatrix30();
	float GetColorCorrectionMatrix30_Maximum();
	float GetColorCorrectionMatrix30_Minimum();
	float GetColorCorrectionMatrix30_Increment();
	
	void SetColorCorrectionMatrix30(float ColorCorrectionMatrix30);
	 
	// Color Correction Matrix element [3][1] (XI_PRM_CC_MATRIX_31)
	
	float GetColorCorrectionMatrix31();
	float GetColorCorrectionMatrix31_Maximum();
	float GetColorCorrectionMatrix31_Minimum();
	float GetColorCorrectionMatrix31_Increment();
	
	void SetColorCorrectionMatrix31(float ColorCorrectionMatrix31);
	 
	// Color Correction Matrix element [3][2] (XI_PRM_CC_MATRIX_32)
	
	float GetColorCorrectionMatrix32();
	float GetColorCorrectionMatrix32_Maximum();
	float GetColorCorrectionMatrix32_Minimum();
	float GetColorCorrectionMatrix32_Increment();
	
	void SetColorCorrectionMatrix32(float ColorCorrectionMatrix32);
	 
	// Color Correction Matrix element [3][3] (XI_PRM_CC_MATRIX_33)
	
	float GetColorCorrectionMatrix33();
	float GetColorCorrectionMatrix33_Maximum();
	float GetColorCorrectionMatrix33_Minimum();
	float GetColorCorrectionMatrix33_Increment();
	
	void SetColorCorrectionMatrix33(float ColorCorrectionMatrix33);
	 
	// Set default Color Correction Matrix (XI_PRM_DEFAULT_CC_MATRIX)
	
	void SetColorCorrectionMatrixDefault(int ColorCorrectionMatrixDefault);
	 
	// Normalize color correction matrix (XI_PRM_CC_MATRIX_NORM)
	
	bool IsColorCorrectionMatrixNorm();
	
	void EnableColorCorrectionMatrixNorm();
	void DisableColorCorrectionMatrixNorm();
	

	//-------------------------------------------------------------------------------------------------------------------
	// ---- Parameter Group: Device IO
	//-------------------------------------------------------------------------------------------------------------------

	 
	// Defines source of trigger. (XI_PRM_TRG_SOURCE)
	XI_TRG_SOURCE GetTriggerSource();
	XI_TRG_SOURCE GetTriggerSource_Maximum();
	XI_TRG_SOURCE GetTriggerSource_Minimum();
	int GetTriggerSource_Increment();
	
	void SetTriggerSource(XI_TRG_SOURCE TriggerSource);
	 
	// Generates an internal trigger. XI_PRM_TRG_SOURCE must be set to TRG_SOFTWARE. (XI_PRM_TRG_SOFTWARE)
	
	void SetTriggerSoftware(int TriggerSoftware);
	 
	// Selects the type of trigger. (XI_PRM_TRG_SELECTOR)
	XI_TRG_SELECTOR GetTriggerSelector();
	XI_TRG_SELECTOR GetTriggerSelector_Maximum();
	XI_TRG_SELECTOR GetTriggerSelector_Minimum();
	int GetTriggerSelector_Increment();
	
	void SetTriggerSelector(XI_TRG_SELECTOR TriggerSelector);
	 
	// The mode of Trigger Overlap. This influences of trigger acception/rejection policy (XI_PRM_TRG_OVERLAP)
	XI_TRG_OVERLAP GetTriggerOverlap();
	XI_TRG_OVERLAP GetTriggerOverlap_Maximum();
	XI_TRG_OVERLAP GetTriggerOverlap_Minimum();
	int GetTriggerOverlap_Increment();
	
	void SetTriggerOverlap(XI_TRG_OVERLAP TriggerOverlap);
	 
	// Sets number of frames acquired by burst. This burst is used only if trigger is set to FrameBurstStart (XI_PRM_ACQ_FRAME_BURST_COUNT)
	int GetAcquisitionFrameBurstCount();
	int GetAcquisitionFrameBurstCount_Maximum();
	int GetAcquisitionFrameBurstCount_Minimum();
	int GetAcquisitionFrameBurstCount_Increment();
	void SetAcquisitionFrameBurstCount(int AcquisitionFrameBurstCount);
	 
	// Current value of the device timestamp counter (XI_PRM_TIMESTAMP)
	uint64_t GetTimestamp();
	uint64_t GetTimestamp_Maximum();
	uint64_t GetTimestamp_Minimum();
	uint64_t GetTimestamp_Increment();

	//-------------------------------------------------------------------------------------------------------------------
	// ---- Parameter Group: GPIO Setup
	//-------------------------------------------------------------------------------------------------------------------

	 
	// Selects GPI (XI_PRM_GPI_SELECTOR)
	XI_GPI_SELECTOR GetGPISelector();
	XI_GPI_SELECTOR GetGPISelector_Maximum();
	XI_GPI_SELECTOR GetGPISelector_Minimum();
	int GetGPISelector_Increment();
	
	void SetGPISelector(XI_GPI_SELECTOR GPISelector);
	 
	// Defines GPI functionality (XI_PRM_GPI_MODE)
	XI_GPI_MODE GetGPIMode();
	XI_GPI_MODE GetGPIMode_Maximum();
	XI_GPI_MODE GetGPIMode_Minimum();
	int GetGPIMode_Increment();
	
	void SetGPIMode(XI_GPI_MODE GPIMode);
	 
	// GPI level (XI_PRM_GPI_LEVEL)
	int GetGPILevel();
	int GetGPILevel_Maximum();
	int GetGPILevel_Minimum();
	int GetGPILevel_Increment(); 
	// GPI Level at image exposure start (XI_PRM_GPI_LEVEL_AT_IMAGE_EXP_START)
	int GetGPILevelAtImageExpStart();
	int GetGPILevelAtImageExpStart_Maximum();
	int GetGPILevelAtImageExpStart_Minimum();
	int GetGPILevelAtImageExpStart_Increment(); 
	// GPI Level at image exposure end (XI_PRM_GPI_LEVEL_AT_IMAGE_EXP_END)
	int GetGPILevelAtImageExpEnd();
	int GetGPILevelAtImageExpEnd_Maximum();
	int GetGPILevelAtImageExpEnd_Minimum();
	int GetGPILevelAtImageExpEnd_Increment(); 
	// Selects GPO (XI_PRM_GPO_SELECTOR)
	XI_GPO_SELECTOR GetGPOSelector();
	XI_GPO_SELECTOR GetGPOSelector_Maximum();
	XI_GPO_SELECTOR GetGPOSelector_Minimum();
	int GetGPOSelector_Increment();
	
	void SetGPOSelector(XI_GPO_SELECTOR GPOSelector);
	 
	// Defines GPO functionality (XI_PRM_GPO_MODE)
	XI_GPO_MODE GetGPOMode();
	XI_GPO_MODE GetGPOMode_Maximum();
	XI_GPO_MODE GetGPOMode_Minimum();
	int GetGPOMode_Increment();
	
	void SetGPOMode(XI_GPO_MODE GPOMode);
	 
	// Selects LED (XI_PRM_LED_SELECTOR)
	XI_LED_SELECTOR GetLEDSelector();
	XI_LED_SELECTOR GetLEDSelector_Maximum();
	XI_LED_SELECTOR GetLEDSelector_Minimum();
	int GetLEDSelector_Increment();
	
	void SetLEDSelector(XI_LED_SELECTOR LEDSelector);
	 
	// Defines LED functionality (XI_PRM_LED_MODE)
	XI_LED_MODE GetLEDMode();
	XI_LED_MODE GetLEDMode_Maximum();
	XI_LED_MODE GetLEDMode_Minimum();
	int GetLEDMode_Increment();
	
	void SetLEDMode(XI_LED_MODE LEDMode);
	 
	// Enable/Disable debounce to selected GPI (XI_PRM_DEBOUNCE_EN)
	
	bool IsGPIDebounce();
	
	void EnableGPIDebounce();
	void DisableGPIDebounce();
	

	//-------------------------------------------------------------------------------------------------------------------
	// ---- Parameter Group: Debounce Setup
	//-------------------------------------------------------------------------------------------------------------------

	 
	// Debounce time (x * 10us) (XI_PRM_DEBOUNCE_T0)
	int GetGPIDebounceFallingEdge();
	int GetGPIDebounceFallingEdge_Maximum();
	int GetGPIDebounceFallingEdge_Minimum();
	int GetGPIDebounceFallingEdge_Increment();
	void SetGPIDebounceFallingEdge(int GPIDebounceFallingEdge);
	 
	// Debounce time (x * 10us) (XI_PRM_DEBOUNCE_T1)
	int GetGPIDebounceRisingEdge();
	int GetGPIDebounceRisingEdge_Maximum();
	int GetGPIDebounceRisingEdge_Minimum();
	int GetGPIDebounceRisingEdge_Increment();
	void SetGPIDebounceRisingEdge(int GPIDebounceRisingEdge);
	 
	// Debounce polarity (pol = 1 t0 - falling edge, t1 - rising edge) (XI_PRM_DEBOUNCE_POL)
	int GetGPIDebouncePolarity();
	int GetGPIDebouncePolarity_Maximum();
	int GetGPIDebouncePolarity_Minimum();
	int GetGPIDebouncePolarity_Increment();
	void SetGPIDebouncePolarity(int GPIDebouncePolarity);
	

	//-------------------------------------------------------------------------------------------------------------------
	// ---- Parameter Group: Lens Control
	//-------------------------------------------------------------------------------------------------------------------

	 
	// Status of lens control interface. This shall be set to XI_ON before any Lens operations. (XI_PRM_LENS_MODE)
	
	bool IsLensMode();
	
	void EnableLensMode();
	void DisableLensMode();
	 
	// Current lens aperture value in stops. Examples: 2.8, 4, 5.6, 8, 11 (XI_PRM_LENS_APERTURE_VALUE)
	
	float GetLensApertureValue();
	float GetLensApertureValue_Maximum();
	float GetLensApertureValue_Minimum();
	float GetLensApertureValue_Increment();
	
	void SetLensApertureValue(float LensApertureValue);
	 
	// Current aperture index as reported by lens. (XI_PRM_LENS_APERTURE_INDEX)
	int GetLensApertureIndex();
	int GetLensApertureIndex_Maximum();
	int GetLensApertureIndex_Minimum();
	int GetLensApertureIndex_Increment();
	void SetLensApertureIndex(int LensApertureIndex);
	 
	// Lens current focus movement value to be used by XI_PRM_LENS_FOCUS_MOVE in motor steps. (XI_PRM_LENS_FOCUS_MOVEMENT_VALUE)
	int GetLensFocusMovementValue();
	int GetLensFocusMovementValue_Maximum();
	int GetLensFocusMovementValue_Minimum();
	int GetLensFocusMovementValue_Increment();
	void SetLensFocusMovementValue(int LensFocusMovementValue);
	 
	// Moves lens focus motor by steps set in XI_PRM_LENS_FOCUS_MOVEMENT_VALUE. (XI_PRM_LENS_FOCUS_MOVE)
	
	void SetLensFocusMove(int LensFocusMove);
	 
	// Lens focal distance in mm. (XI_PRM_LENS_FOCAL_LENGTH)
	
	float GetLensFocalLength();
	float GetLensFocalLength_Maximum();
	float GetLensFocalLength_Minimum();
	float GetLensFocalLength_Increment();
	 
	// Selects the current feature which is accessible by XI_PRM_LENS_FEATURE. (XI_PRM_LENS_FEATURE_SELECTOR)
	XI_LENS_FEATURE GetLensFeatureSelector();
	XI_LENS_FEATURE GetLensFeatureSelector_Maximum();
	XI_LENS_FEATURE GetLensFeatureSelector_Minimum();
	int GetLensFeatureSelector_Increment();
	
	void SetLensFeatureSelector(XI_LENS_FEATURE LensFeatureSelector);
	 
	// Allows access to lens feature value currently selected by XI_PRM_LENS_FEATURE_SELECTOR. (XI_PRM_LENS_FEATURE)
	
	float GetLensFeature();
	float GetLensFeature_Maximum();
	float GetLensFeature_Minimum();
	float GetLensFeature_Increment();
	
	void SetLensFeature(float LensFeature);
	

	//-------------------------------------------------------------------------------------------------------------------
	// ---- Parameter Group: Device info parameters
	//-------------------------------------------------------------------------------------------------------------------

	 
	// Return device name (XI_PRM_DEVICE_NAME)
	
	void GetCameraName(char* buffer, int buffer_lenght);
	 
	// Return device type (XI_PRM_DEVICE_TYPE)
	
	void GetCameraType(char* buffer, int buffer_lenght);
	 
	// Return device model id (XI_PRM_DEVICE_MODEL_ID)
	int GetModelId();
	int GetModelId_Maximum();
	int GetModelId_Minimum();
	int GetModelId_Increment(); 
	// Return device sensor model id (XI_PRM_SENSOR_MODEL_ID)
	int GetSensorId();
	int GetSensorId_Maximum();
	int GetSensorId_Minimum();
	int GetSensorId_Increment(); 
	// Return device serial number (XI_PRM_DEVICE_SN)
	
	void GetSerialNumber(char* buffer, int buffer_lenght);
	 
	// Return sensor serial number (XI_PRM_DEVICE_SENS_SN)
	
	void GetSensorSerialNumber(char* buffer, int buffer_lenght);
	 
	// Return device system instance path. (XI_PRM_DEVICE_INSTANCE_PATH)
	
	void GetDevicePath(char* buffer, int buffer_lenght);
	 
	// Represents the location of the device in the device tree. (XI_PRM_DEVICE_LOCATION_PATH)
	
	void GetDeviceLocPath(char* buffer, int buffer_lenght);
	 
	// Return custom ID of camera. (XI_PRM_DEVICE_USER_ID)
	
	void GetUserId(char* buffer, int buffer_lenght);
	 
	// Return device capability description XML. (XI_PRM_DEVICE_MANIFEST)
	
	void GetDeviceManifest(char* buffer, int buffer_lenght);
	 
	// User image data at image header to track parameters synchronization. (XI_PRM_IMAGE_USER_DATA)
	int GetImageUserData();
	int GetImageUserData_Maximum();
	int GetImageUserData_Minimum();
	int GetImageUserData_Increment();
	void SetImageUserData(int ImageUserData);
	

	//-------------------------------------------------------------------------------------------------------------------
	// ---- Parameter Group: Device acquisition settings
	//-------------------------------------------------------------------------------------------------------------------

	 
	// The alpha channel of RGB32 output image format. (XI_PRM_IMAGE_DATA_FORMAT_RGB32_ALPHA)
	int GetImageDataFormatRGB32Alpha();
	int GetImageDataFormatRGB32Alpha_Maximum();
	int GetImageDataFormatRGB32Alpha_Minimum();
	int GetImageDataFormatRGB32Alpha_Increment();
	void SetImageDataFormatRGB32Alpha(int ImageDataFormatRGB32Alpha);
	 
	// Buffer size in bytes sufficient for output image returned by xiGetImage (XI_PRM_IMAGE_PAYLOAD_SIZE)
	int GetImagePayloadSize();
	int GetImagePayloadSize_Maximum();
	int GetImagePayloadSize_Minimum();
	int GetImagePayloadSize_Increment(); 
	// Current format of pixels on transport layer. (XI_PRM_TRANSPORT_PIXEL_FORMAT)
	XI_GenTL_Image_Format_e GetTransportPixelFormat();
	XI_GenTL_Image_Format_e GetTransportPixelFormat_Maximum();
	XI_GenTL_Image_Format_e GetTransportPixelFormat_Minimum();
	int GetTransportPixelFormat_Increment();
	
	void SetTransportPixelFormat(XI_GenTL_Image_Format_e TransportPixelFormat);
	 
	// Target selector for data - CPU RAM or GPU RAM (XI_PRM_TRANSPORT_DATA_TARGET)
	XI_TRANSPORT_DATA_TARGET_MODE GetTransportDataTarget();
	XI_TRANSPORT_DATA_TARGET_MODE GetTransportDataTarget_Maximum();
	XI_TRANSPORT_DATA_TARGET_MODE GetTransportDataTarget_Minimum();
	int GetTransportDataTarget_Increment();
	
	void SetTransportDataTarget(XI_TRANSPORT_DATA_TARGET_MODE TransportDataTarget);
	 
	// Sensor clock frequency in Hz. (XI_PRM_SENSOR_CLOCK_FREQ_HZ)
	
	float GetSensorClockFrequencyHz();
	float GetSensorClockFrequencyHz_Maximum();
	float GetSensorClockFrequencyHz_Minimum();
	float GetSensorClockFrequencyHz_Increment();
	
	void SetSensorClockFrequencyHz(float SensorClockFrequencyHz);
	 
	// Sensor clock frequency index. Sensor with selected frequencies have possibility to set the frequency only by this index. (XI_PRM_SENSOR_CLOCK_FREQ_INDEX)
	int GetSensorClockFrequencyIndex();
	int GetSensorClockFrequencyIndex_Maximum();
	int GetSensorClockFrequencyIndex_Minimum();
	int GetSensorClockFrequencyIndex_Increment();
	void SetSensorClockFrequencyIndex(int SensorClockFrequencyIndex);
	 
	// Number of output channels from sensor used for data transfer. (XI_PRM_SENSOR_OUTPUT_CHANNEL_COUNT)
	XI_SENSOR_OUTPUT_CHANNEL_COUNT GetSensorOutputChannelCount();
	XI_SENSOR_OUTPUT_CHANNEL_COUNT GetSensorOutputChannelCount_Maximum();
	XI_SENSOR_OUTPUT_CHANNEL_COUNT GetSensorOutputChannelCount_Minimum();
	int GetSensorOutputChannelCount_Increment();
	
	void SetSensorOutputChannelCount(XI_SENSOR_OUTPUT_CHANNEL_COUNT SensorOutputChannelCount);
	 
	// Define framerate in Hz (XI_PRM_FRAMERATE)
	
	float GetFrameRate();
	float GetFrameRate_Maximum();
	float GetFrameRate_Minimum();
	float GetFrameRate_Increment();
	
	void SetFrameRate(float FrameRate);
	 
	// Select counter (XI_PRM_COUNTER_SELECTOR)
	XI_COUNTER_SELECTOR GetCounterSelector();
	XI_COUNTER_SELECTOR GetCounterSelector_Maximum();
	XI_COUNTER_SELECTOR GetCounterSelector_Minimum();
	int GetCounterSelector_Increment();
	
	void SetCounterSelector(XI_COUNTER_SELECTOR CounterSelector);
	 
	// Counter status (XI_PRM_COUNTER_VALUE)
	int GetCounterValue();
	int GetCounterValue_Maximum();
	int GetCounterValue_Minimum();
	int GetCounterValue_Increment(); 
	// Type of sensor frames timing. (XI_PRM_ACQ_TIMING_MODE)
	XI_ACQ_TIMING_MODE GetAcquisitionTimingMode();
	XI_ACQ_TIMING_MODE GetAcquisitionTimingMode_Maximum();
	XI_ACQ_TIMING_MODE GetAcquisitionTimingMode_Minimum();
	int GetAcquisitionTimingMode_Increment();
	
	void SetAcquisitionTimingMode(XI_ACQ_TIMING_MODE AcquisitionTimingMode);
	 
	// Measure available interface bandwidth (in Megabits) (XI_PRM_AVAILABLE_BANDWIDTH)
	int GetBandwidthAvailable();
	int GetBandwidthAvailable_Maximum();
	int GetBandwidthAvailable_Minimum();
	int GetBandwidthAvailable_Increment(); 
	// Data move policy (XI_PRM_BUFFER_POLICY)
	XI_BP GetBufferPolicy();
	XI_BP GetBufferPolicy_Maximum();
	XI_BP GetBufferPolicy_Minimum();
	int GetBufferPolicy_Increment();
	
	void SetBufferPolicy(XI_BP BufferPolicy);
	 
	// Activates LUT. (XI_PRM_LUT_EN)
	
	bool IsLookUpTable();
	
	void EnableLookUpTable();
	void DisableLookUpTable();
	 
	// Control the index (offset) of the coefficient to access in the LUT. (XI_PRM_LUT_INDEX)
	int GetLookUpTableIndex();
	int GetLookUpTableIndex_Maximum();
	int GetLookUpTableIndex_Minimum();
	int GetLookUpTableIndex_Increment();
	void SetLookUpTableIndex(int LookUpTableIndex);
	 
	// Value at entry LUTIndex of the LUT (XI_PRM_LUT_VALUE)
	int GetLookUpTableValue();
	int GetLookUpTableValue_Maximum();
	int GetLookUpTableValue_Minimum();
	int GetLookUpTableValue_Increment();
	void SetLookUpTableValue(int LookUpTableValue);
	 
	// Specifies the delay in microseconds (us) to apply after the trigger reception before activating it. (XI_PRM_TRG_DELAY)
	int GetTriggerDelay();
	int GetTriggerDelay_Maximum();
	int GetTriggerDelay_Minimum();
	int GetTriggerDelay_Increment();
	void SetTriggerDelay(int TriggerDelay);
	 
	// Defines how TimeStamp reset engine will be armed (XI_PRM_TS_RST_MODE)
	XI_TS_RST_MODE GetTimestampResetMode();
	XI_TS_RST_MODE GetTimestampResetMode_Maximum();
	XI_TS_RST_MODE GetTimestampResetMode_Minimum();
	int GetTimestampResetMode_Increment();
	
	void SetTimestampResetMode(XI_TS_RST_MODE TimestampResetMode);
	 
	// Defines which source will be used for timestamp reset. Writing this parameter will trigger settings of engine (arming) (XI_PRM_TS_RST_SOURCE)
	XI_TS_RST_SOURCE GetTimestampResetSource();
	XI_TS_RST_SOURCE GetTimestampResetSource_Maximum();
	XI_TS_RST_SOURCE GetTimestampResetSource_Minimum();
	int GetTimestampResetSource_Increment();
	
	void SetTimestampResetSource(XI_TS_RST_SOURCE TimestampResetSource);
	

	//-------------------------------------------------------------------------------------------------------------------
	// ---- Parameter Group: Extended Device parameters
	//-------------------------------------------------------------------------------------------------------------------

	 
	// Returns 1 if camera connected and works properly. (XI_PRM_IS_DEVICE_EXIST)
	
	bool IsExist();
	 
	// Acquisition buffer size in buffer_size_unit. Default bytes. (XI_PRM_ACQ_BUFFER_SIZE)
	int GetAcquisitionBufferSizeBytes();
	int GetAcquisitionBufferSizeBytes_Maximum();
	int GetAcquisitionBufferSizeBytes_Minimum();
	int GetAcquisitionBufferSizeBytes_Increment();
	void SetAcquisitionBufferSizeBytes(int AcquisitionBufferSizeBytes);
	 
	// Acquisition buffer size unit in bytes. Default 1. E.g. Value 1024 means that buffer_size is in KiBytes (XI_PRM_ACQ_BUFFER_SIZE_UNIT)
	int GetAcquisitionBufferSizeBytesUnit();
	int GetAcquisitionBufferSizeBytesUnit_Maximum();
	int GetAcquisitionBufferSizeBytesUnit_Minimum();
	int GetAcquisitionBufferSizeBytesUnit_Increment();
	void SetAcquisitionBufferSizeBytesUnit(int AcquisitionBufferSizeBytesUnit);
	 
	// Acquisition transport buffer size in bytes (XI_PRM_ACQ_TRANSPORT_BUFFER_SIZE)
	int GetAcquisitionTransportBufferSizeBytes();
	int GetAcquisitionTransportBufferSizeBytes_Maximum();
	int GetAcquisitionTransportBufferSizeBytes_Minimum();
	int GetAcquisitionTransportBufferSizeBytes_Increment();
	void SetAcquisitionTransportBufferSizeBytes(int AcquisitionTransportBufferSizeBytes);
	 
	// Acquisition transport packet size in bytes (XI_PRM_ACQ_TRANSPORT_PACKET_SIZE)
	int GetAcquisitionTransportPacketSizeBytes();
	int GetAcquisitionTransportPacketSizeBytes_Maximum();
	int GetAcquisitionTransportPacketSizeBytes_Minimum();
	int GetAcquisitionTransportPacketSizeBytes_Increment();
	void SetAcquisitionTransportPacketSizeBytes(int AcquisitionTransportPacketSizeBytes);
	 
	// Queue of field/frame buffers (XI_PRM_BUFFERS_QUEUE_SIZE)
	int GetAcquisitionQueueImagesCount();
	int GetAcquisitionQueueImagesCount_Maximum();
	int GetAcquisitionQueueImagesCount_Minimum();
	int GetAcquisitionQueueImagesCount_Increment();
	void SetAcquisitionQueueImagesCount(int AcquisitionQueueImagesCount);
	 
	// Total number of buffers to be committed to transport layer. Increasing can enhance transport capacity. E.g. on USB (XI_PRM_ACQ_TRANSPORT_BUFFER_COMMIT)
	int GetAcquisitionTransportBufferCommit();
	int GetAcquisitionTransportBufferCommit_Maximum();
	int GetAcquisitionTransportBufferCommit_Minimum();
	int GetAcquisitionTransportBufferCommit_Increment();
	void SetAcquisitionTransportBufferCommit(int AcquisitionTransportBufferCommit);
	 
	// GetImage returns most recent frame (XI_PRM_RECENT_FRAME)
	
	bool IsSelectRecentImage();
	
	void EnableSelectRecentImage();
	void DisableSelectRecentImage();
	 
	// Resets the camera to default state. (XI_PRM_DEVICE_RESET)
	
	void SetDeviceReset(int DeviceReset);
	 
	// Enable/disable the Concatenated Images in One Buffer feature (XI_PRM_CONCAT_IMG_MODE)
	
	bool IsConcatenatedImagesMode();
	
	void EnableConcatenatedImagesMode();
	void DisableConcatenatedImagesMode();
	 
	// Number of Concatenated Images in One Buffer (XI_PRM_CONCAT_IMG_COUNT)
	int GetConcatenatedImagesCount();
	int GetConcatenatedImagesCount_Maximum();
	int GetConcatenatedImagesCount_Minimum();
	int GetConcatenatedImagesCount_Increment();
	void SetConcatenatedImagesCount(int ConcatenatedImagesCount);
	 
	// Offset between images data in transport buffer when feature Concatenated Images in One Buffer is enabled (XI_PRM_CONCAT_IMG_TRANSPORT_IMG_OFFSET)
	int GetConcatenatedImagesTransportImgOffset();
	int GetConcatenatedImagesTransportImgOffset_Maximum();
	int GetConcatenatedImagesTransportImgOffset_Minimum();
	int GetConcatenatedImagesTransportImgOffset_Increment(); 
	// Select Probe (XI_PRM_PROBE_SELECTOR)
	XI_PROBE_SELECTOR GetProbeSelector();
	XI_PROBE_SELECTOR GetProbeSelector_Maximum();
	XI_PROBE_SELECTOR GetProbeSelector_Minimum();
	int GetProbeSelector_Increment();
	
	void SetProbeSelector(XI_PROBE_SELECTOR ProbeSelector);
	 
	// Returns Value of the selected Probe (XI_PRM_PROBE_VALUE)
	
	float GetProbeValue();
	float GetProbeValue_Maximum();
	float GetProbeValue_Minimum();
	float GetProbeValue_Increment();
	

	//-------------------------------------------------------------------------------------------------------------------
	// ---- Parameter Group: Sensor Defects Correction
	//-------------------------------------------------------------------------------------------------------------------

	 
	// Correction of column FPN (XI_PRM_COLUMN_FPN_CORRECTION)
	XI_SWITCH GetColumnFpnCorrection();
	XI_SWITCH GetColumnFpnCorrection_Maximum();
	XI_SWITCH GetColumnFpnCorrection_Minimum();
	int GetColumnFpnCorrection_Increment();
	
	void SetColumnFpnCorrection(XI_SWITCH ColumnFpnCorrection);
	 
	// Correction of row FPN (XI_PRM_ROW_FPN_CORRECTION)
	XI_SWITCH GetRowFpnCorrection();
	XI_SWITCH GetRowFpnCorrection_Maximum();
	XI_SWITCH GetRowFpnCorrection_Minimum();
	int GetRowFpnCorrection_Increment();
	
	void SetRowFpnCorrection(XI_SWITCH RowFpnCorrection);
	 
	// Correction of column black offset (XI_PRM_COLUMN_BLACK_OFFSET_CORRECTION)
	XI_SWITCH GetColumnBlackOffsetCorrection();
	XI_SWITCH GetColumnBlackOffsetCorrection_Maximum();
	XI_SWITCH GetColumnBlackOffsetCorrection_Minimum();
	int GetColumnBlackOffsetCorrection_Increment();
	
	void SetColumnBlackOffsetCorrection(XI_SWITCH ColumnBlackOffsetCorrection);
	 
	// Correction of row black offset (XI_PRM_ROW_BLACK_OFFSET_CORRECTION)
	XI_SWITCH GetRowBlackOffsetCorrection();
	XI_SWITCH GetRowBlackOffsetCorrection_Maximum();
	XI_SWITCH GetRowBlackOffsetCorrection_Minimum();
	int GetRowBlackOffsetCorrection_Increment();
	
	void SetRowBlackOffsetCorrection(XI_SWITCH RowBlackOffsetCorrection);
	

	//-------------------------------------------------------------------------------------------------------------------
	// ---- Parameter Group: Sensor features
	//-------------------------------------------------------------------------------------------------------------------

	 
	// Current sensor mode. Allows to select sensor mode by one integer. Setting of this parameter affects: image dimensions and downsampling. (XI_PRM_SENSOR_MODE)
	XI_SENSOR_MODE GetSensorMode();
	XI_SENSOR_MODE GetSensorMode_Maximum();
	XI_SENSOR_MODE GetSensorMode_Minimum();
	int GetSensorMode_Increment();
	
	void SetSensorMode(XI_SENSOR_MODE SensorMode);
	 
	// Enable High Dynamic Range feature. (XI_PRM_HDR)
	
	bool IsHDR();
	
	void EnableHDR();
	void DisableHDR();
	 
	// The number of kneepoints in the PWLR. (XI_PRM_HDR_KNEEPOINT_COUNT)
	int GetHDR_KNEEPOINT_COUNT();
	int GetHDR_KNEEPOINT_COUNT_Maximum();
	int GetHDR_KNEEPOINT_COUNT_Minimum();
	int GetHDR_KNEEPOINT_COUNT_Increment();
	void SetHDR_KNEEPOINT_COUNT(int HDR_KNEEPOINT_COUNT);
	 
	// position of first kneepoint(in % of XI_PRM_EXPOSURE) (XI_PRM_HDR_T1)
	int GetHDRTimeSlope1();
	int GetHDRTimeSlope1_Maximum();
	int GetHDRTimeSlope1_Minimum();
	int GetHDRTimeSlope1_Increment();
	void SetHDRTimeSlope1(int HDRTimeSlope1);
	 
	// position of second kneepoint (in % of XI_PRM_EXPOSURE) (XI_PRM_HDR_T2)
	int GetHDRTimeSlope2();
	int GetHDRTimeSlope2_Maximum();
	int GetHDRTimeSlope2_Minimum();
	int GetHDRTimeSlope2_Increment();
	void SetHDRTimeSlope2(int HDRTimeSlope2);
	 
	// value of first kneepoint (% of sensor saturation) (XI_PRM_KNEEPOINT1)
	int GetHDRKnee1Percent();
	int GetHDRKnee1Percent_Maximum();
	int GetHDRKnee1Percent_Minimum();
	int GetHDRKnee1Percent_Increment();
	void SetHDRKnee1Percent(int HDRKnee1Percent);
	 
	// value of second kneepoint (% of sensor saturation) (XI_PRM_KNEEPOINT2)
	int GetHDRKnee2Percent();
	int GetHDRKnee2Percent_Maximum();
	int GetHDRKnee2Percent_Minimum();
	int GetHDRKnee2Percent_Increment();
	void SetHDRKnee2Percent(int HDRKnee2Percent);
	 
	// Last image black level counts (same as in XI_IMG). Setting can be used only for Offline Processing. (XI_PRM_IMAGE_BLACK_LEVEL)
	int GetImageBlackLevel();
	int GetImageBlackLevel_Maximum();
	int GetImageBlackLevel_Minimum();
	int GetImageBlackLevel_Increment(); 
	// Defines image area of sensor as output. (XI_PRM_IMAGE_AREA)
	XI_IMAGE_AREA_SELECTOR GetImageArea();
	XI_IMAGE_AREA_SELECTOR GetImageArea_Maximum();
	XI_IMAGE_AREA_SELECTOR GetImageArea_Minimum();
	int GetImageArea_Increment();
	
	void SetImageArea(XI_IMAGE_AREA_SELECTOR ImageArea);
	 
	// Sets DualADC Mode (XI_PRM_DUAL_ADC_MODE)
	XI_DUAL_ADC_MODE GetDualADCMode();
	XI_DUAL_ADC_MODE GetDualADCMode_Maximum();
	XI_DUAL_ADC_MODE GetDualADCMode_Minimum();
	int GetDualADCMode_Increment();
	
	void SetDualADCMode(XI_DUAL_ADC_MODE DualADCMode);
	 
	// Sets DualADC Gain Ratio in dB (XI_PRM_DUAL_ADC_GAIN_RATIO)
	
	float GetDualADCGainRatio();
	float GetDualADCGainRatio_Maximum();
	float GetDualADCGainRatio_Minimum();
	float GetDualADCGainRatio_Increment();
	
	void SetDualADCGainRatio(float DualADCGainRatio);
	 
	// Sets DualADC Threshold value (XI_PRM_DUAL_ADC_THRESHOLD)
	int GetDualADCThreshold();
	int GetDualADCThreshold_Maximum();
	int GetDualADCThreshold_Minimum();
	int GetDualADCThreshold_Increment();
	void SetDualADCThreshold(int DualADCThreshold);
	 
	// Sets Compression Region Selector (XI_PRM_COMPRESSION_REGION_SELECTOR)
	int GetCompressionRegionSelector();
	int GetCompressionRegionSelector_Maximum();
	int GetCompressionRegionSelector_Minimum();
	int GetCompressionRegionSelector_Increment();
	void SetCompressionRegionSelector(int CompressionRegionSelector);
	 
	// Sets Compression Region Start (XI_PRM_COMPRESSION_REGION_START)
	
	float GetCompressionRegionStart();
	float GetCompressionRegionStart_Maximum();
	float GetCompressionRegionStart_Minimum();
	float GetCompressionRegionStart_Increment();
	
	void SetCompressionRegionStart(float CompressionRegionStart);
	 
	// Sets Compression Region Gain (XI_PRM_COMPRESSION_REGION_GAIN)
	
	float GetCompressionRegionGain();
	float GetCompressionRegionGain_Maximum();
	float GetCompressionRegionGain_Minimum();
	float GetCompressionRegionGain_Increment();
	
	void SetCompressionRegionGain(float CompressionRegionGain);
	

	//-------------------------------------------------------------------------------------------------------------------
	// ---- Parameter Group: Version info
	//-------------------------------------------------------------------------------------------------------------------

	 
	// Selects module/unit, which version we get. (XI_PRM_VERSION_SELECTOR)
	XI_VERSION GetVersionSelector();
	XI_VERSION GetVersionSelector_Maximum();
	XI_VERSION GetVersionSelector_Minimum();
	int GetVersionSelector_Increment();
	
	void SetVersionSelector(XI_VERSION VersionSelector);
	 
	// Returns version of selected module/unit(XI_PRM_VERSION_SELECTOR). (XI_PRM_VERSION)
	
	void GetVersion(char* buffer, int buffer_lenght);
	 
	// Returns version of API. (XI_PRM_API_VERSION)
	
	void GetVersionAPI(char* buffer, int buffer_lenght);
	 
	// Returns version of current device driver. (XI_PRM_DRV_VERSION)
	
	void GetVersionDriver(char* buffer, int buffer_lenght);
	 
	// Returns version of MCU1 firmware. (XI_PRM_MCU1_VERSION)
	
	void GetVersionMCU1(char* buffer, int buffer_lenght);
	 
	// Returns version of MCU2 firmware. (XI_PRM_MCU2_VERSION)
	
	void GetVersionMCU2(char* buffer, int buffer_lenght);
	 
	// Returns version of MCU3 firmware. (XI_PRM_MCU3_VERSION)
	
	void GetVersionMCU3(char* buffer, int buffer_lenght);
	 
	// Returns version of FPGA firmware currently running. (XI_PRM_FPGA1_VERSION)
	
	void GetVersionFPGA1(char* buffer, int buffer_lenght);
	 
	// Returns version of XML manifest. (XI_PRM_XMLMAN_VERSION)
	
	void GetVersionXMLMAN(char* buffer, int buffer_lenght);
	 
	// Returns hardware revision number. (XI_PRM_HW_REVISION)
	
	void GetHWRevision(char* buffer, int buffer_lenght);
	 
	// Returns version of factory set. (XI_PRM_FACTORY_SET_VERSION)
	
	void GetVersionFactSet(char* buffer, int buffer_lenght);
	

	//-------------------------------------------------------------------------------------------------------------------
	// ---- Parameter Group: API features
	//-------------------------------------------------------------------------------------------------------------------

	 
	// Set debug level (XI_PRM_DEBUG_LEVEL)
	XI_DEBUG_LEVEL GetDebugLevel();
	XI_DEBUG_LEVEL GetDebugLevel_Maximum();
	XI_DEBUG_LEVEL GetDebugLevel_Minimum();
	int GetDebugLevel_Increment();
	
	void SetDebugLevel(XI_DEBUG_LEVEL DebugLevel);
	 
	// Automatic bandwidth calculation, (XI_PRM_AUTO_BANDWIDTH_CALCULATION)
	
	bool IsAutoBandwidthCalculation();
	
	void EnableAutoBandwidthCalculation();
	void DisableAutoBandwidthCalculation();
	 
	// Enables (2015/FAPI) processing chain for MQ MU cameras. If disabled - legacy processing 2006 is used. (XI_PRM_NEW_PROCESS_CHAIN_ENABLE)
	
	bool IsNewProcessChainEnable();
	
	void EnableNewProcessChainEnable();
	void DisableNewProcessChainEnable();
	 
	// Number of threads per image processor (XI_PRM_PROC_NUM_THREADS)
	int GetProcessorNumberThreads();
	int GetProcessorNumberThreads_Maximum();
	int GetProcessorNumberThreads_Minimum();
	int GetProcessorNumberThreads_Increment();
	void SetProcessorNumberThreads(int ProcessorNumberThreads);
	

	//-------------------------------------------------------------------------------------------------------------------
	// ---- Parameter Group: Camera FFS
	//-------------------------------------------------------------------------------------------------------------------

	 
	// Read file from camera flash filesystem. (XI_PRM_READ_FILE_FFS)
	
	void GetReadFileFFS(char* buffer, int buffer_lenght);
	 
	// Write file to camera flash filesystem. (XI_PRM_WRITE_FILE_FFS)
	
	void GetWriteFileFFS(char* buffer, int buffer_lenght);
	
	void SetWriteFileFFS(char* WriteFileFFS);
	 
	// Set name of file to be written/read from camera FFS. (XI_PRM_FFS_FILE_NAME)
	
	void GetFFSFileName(char* buffer, int buffer_lenght);
	
	void SetFFSFileName(char* FFSFileName);
	 
	// File number. (XI_PRM_FFS_FILE_ID)
	int GetFFSFileId();
	int GetFFSFileId_Maximum();
	int GetFFSFileId_Minimum();
	int GetFFSFileId_Increment(); 
	// Size of file. (XI_PRM_FFS_FILE_SIZE)
	int GetFFSFileSize();
	int GetFFSFileSize_Maximum();
	int GetFFSFileSize_Minimum();
	int GetFFSFileSize_Increment(); 
	// Size of free camera FFS. (XI_PRM_FREE_FFS_SIZE)
	uint64_t GetFreeFFSSize();
	uint64_t GetFreeFFSSize_Maximum();
	uint64_t GetFreeFFSSize_Minimum();
	uint64_t GetFreeFFSSize_Increment(); 
	// Size of used camera FFS. (XI_PRM_USED_FFS_SIZE)
	uint64_t GetUsedFFSSize();
	uint64_t GetUsedFFSSize_Maximum();
	uint64_t GetUsedFFSSize_Minimum();
	uint64_t GetUsedFFSSize_Increment(); 
	// Setting of key enables file operations on some cameras. (XI_PRM_FFS_ACCESS_KEY)
	int GetFFSAccessKey();
	int GetFFSAccessKey_Maximum();
	int GetFFSAccessKey_Minimum();
	int GetFFSAccessKey_Increment();
	void SetFFSAccessKey(int FFSAccessKey);
	

	//-------------------------------------------------------------------------------------------------------------------
	// ---- Parameter Group: APIContextControl
	//-------------------------------------------------------------------------------------------------------------------

	 
	// List of current parameters settings context - parameters with values. Used for offline processing. (XI_PRM_API_CONTEXT_LIST)
	
	void GetApiContextList(char* buffer, int buffer_lenght);
	
	void SetApiContextList(char* ApiContextList);
	

	//-------------------------------------------------------------------------------------------------------------------
	// ---- Parameter Group: Sensor Control
	//-------------------------------------------------------------------------------------------------------------------

	 
	// Selects the current feature which is accessible by XI_PRM_SENSOR_FEATURE_VALUE. (XI_PRM_SENSOR_FEATURE_SELECTOR)
	XI_SENSOR_FEATURE_SELECTOR GetSensorFeatureSelector();
	XI_SENSOR_FEATURE_SELECTOR GetSensorFeatureSelector_Maximum();
	XI_SENSOR_FEATURE_SELECTOR GetSensorFeatureSelector_Minimum();
	int GetSensorFeatureSelector_Increment();
	
	void SetSensorFeatureSelector(XI_SENSOR_FEATURE_SELECTOR SensorFeatureSelector);
	 
	// Allows access to sensor feature value currently selected by XI_PRM_SENSOR_FEATURE_SELECTOR. (XI_PRM_SENSOR_FEATURE_VALUE)
	int GetSensorFeatureValue();
	int GetSensorFeatureValue_Maximum();
	int GetSensorFeatureValue_Minimum();
	int GetSensorFeatureValue_Increment();
	void SetSensorFeatureValue(int SensorFeatureValue);
	

	//-------------------------------------------------------------------------------------------------------------------
	// ---- Parameter Group: Extended Features
	//-------------------------------------------------------------------------------------------------------------------

	 
	// Selects the internal acquisition signal to read using XI_PRM_ACQUISITION_STATUS. (XI_PRM_ACQUISITION_STATUS_SELECTOR)
	XI_ACQUISITION_STATUS_SELECTOR GetAcquisitionStatusSelector();
	XI_ACQUISITION_STATUS_SELECTOR GetAcquisitionStatusSelector_Maximum();
	XI_ACQUISITION_STATUS_SELECTOR GetAcquisitionStatusSelector_Minimum();
	int GetAcquisitionStatusSelector_Increment();
	
	void SetAcquisitionStatusSelector(XI_ACQUISITION_STATUS_SELECTOR AcquisitionStatusSelector);
	 
	// Acquisition status(True/False) (XI_PRM_ACQUISITION_STATUS)
	XI_SWITCH GetAcquisitionStatus();
	XI_SWITCH GetAcquisitionStatus_Maximum();
	XI_SWITCH GetAcquisitionStatus_Minimum();
	int GetAcquisitionStatus_Increment();
	 
	// Data Pipe Unit Selector. (XI_PRM_DP_UNIT_SELECTOR)
	XI_DP_UNIT_SELECTOR GetDataPipeUnitSelector();
	XI_DP_UNIT_SELECTOR GetDataPipeUnitSelector_Maximum();
	XI_DP_UNIT_SELECTOR GetDataPipeUnitSelector_Minimum();
	int GetDataPipeUnitSelector_Increment();
	
	void SetDataPipeUnitSelector(XI_DP_UNIT_SELECTOR DataPipeUnitSelector);
	 
	// Data Pipe Processor Selector. (XI_PRM_DP_PROC_SELECTOR)
	XI_DP_PROC_SELECTOR GetDataPipeProcSelector();
	XI_DP_PROC_SELECTOR GetDataPipeProcSelector_Maximum();
	XI_DP_PROC_SELECTOR GetDataPipeProcSelector_Minimum();
	int GetDataPipeProcSelector_Increment();
	
	void SetDataPipeProcSelector(XI_DP_PROC_SELECTOR DataPipeProcSelector);
	 
	// Data Pipe Processor parameter Selector. (XI_PRM_DP_PARAM_SELECTOR)
	XI_DP_PARAM_SELECTOR GetDataPipeParamSelector();
	XI_DP_PARAM_SELECTOR GetDataPipeParamSelector_Maximum();
	XI_DP_PARAM_SELECTOR GetDataPipeParamSelector_Minimum();
	int GetDataPipeParamSelector_Increment();
	
	void SetDataPipeParamSelector(XI_DP_PARAM_SELECTOR DataPipeParamSelector);
	 
	// Data Pipe processor parameter value (XI_PRM_DP_PARAM_VALUE)
	
	float GetDataPipeParamValue();
	float GetDataPipeParamValue_Maximum();
	float GetDataPipeParamValue_Minimum();
	float GetDataPipeParamValue_Increment();
	
	void SetDataPipeParamValue(float DataPipeParamValue);
	 
	// Enable or disable low level streaming via GenTL. (XI_PRM_GENTL_DATASTREAM_ENABLED)
	
	bool IsGenTLStreamEn();
	
	void EnableGenTLStreamEn();
	void DisableGenTLStreamEn();
	 
	// Get GenTL stream context pointer for low level streaming (XI_PRM_GENTL_DATASTREAM_CONTEXT)
	
	void GetGenTLStreamContext(char* buffer, int buffer_lenght);
	

	//-------------------------------------------------------------------------------------------------------------------
	// ---- Parameter Group: User Set Control
	//-------------------------------------------------------------------------------------------------------------------

	 
	// Selects the feature User Set to load, save or configure. (XI_PRM_USER_SET_SELECTOR)
	XI_USER_SET_SELECTOR GetUserSetSelector();
	XI_USER_SET_SELECTOR GetUserSetSelector_Maximum();
	XI_USER_SET_SELECTOR GetUserSetSelector_Minimum();
	int GetUserSetSelector_Increment();
	
	void SetUserSetSelector(XI_USER_SET_SELECTOR UserSetSelector);
	 
	// Loads the User Set specified by User Set Selector to the device and makes it active. (XI_PRM_USER_SET_LOAD)
	
	void SetUserSetLoad(int UserSetLoad);
	 
	// Selects the feature User Set to load and make active by default when the device is reset. Change might affect default mode in other applications, e.g. CamTool. (XI_PRM_USER_SET_DEFAULT)
	XI_USER_SET_SELECTOR GetUserSetDefault();
	XI_USER_SET_SELECTOR GetUserSetDefault_Maximum();
	XI_USER_SET_SELECTOR GetUserSetDefault_Minimum();
	int GetUserSetDefault_Increment();
	
	void SetUserSetDefault(XI_USER_SET_SELECTOR UserSetDefault);
	


private:

	// internal
	void CheckResult(XI_RETURN res,const char* location);
	void CheckResultParam(XI_RETURN res, const char* location, const char* param);

private:

	HANDLE camera_handle;					// camera handle used by xiAPI.
	int image_timeout_ms;					// acquisition timeout in milliseconds.
	xiAPIplus_Image last_image;				// internal camera xiAPIplus_Image object holding the last acquired image
	bool acquisition_active;				// true when image acquisition is running, false otherwise
	bool is_debug_enabled;					// when enabled internal xiApiPlus checks will be printed into standard output (stdout).
};

// -----------------------------------------------
// class xiAPIplus - ImageProcessing
// -----------------------------------------------

class xiAPIplus_ImageProcessing
{
public:
	xiAPIplus_ImageProcessing();
	~xiAPIplus_ImageProcessing();

	/**
	* SetParam
	* Sets the selected parameter to processing
	* @param[in] prm						parameter name string.
	* @param[in] val						pointer to parameter set value.
	* @param[in] size						size of val.
	* @param[in] type						val data type.
	* @return XI_OK on success, error value otherwise.
	*/
	void SetParam(const char* prm, void* val, DWORD size, XI_PRM_TYPE type);

	/**
	* PushImage
	* Set unprocessed image to processing chain
	* @param[in] first_pixel				First byte of first pixel of image to be processed
	*/
	void PushImage(unsigned char* first_pixel);

	/**
	* PullImage
	* Pulls processed image from processing chain into last_image_
	*/
	void PullImage();

	/**
	* GetImage
	* Returns point to last processed image
	* @return pointer to last_image_ (xiAPIplus_Image)
	*/
	void GetImage(xiAPIplus_Image* ret_image);

	/**
	* CheckResult
	* Check result from xiAPIplus_ImageProcessing call.
	* @param[in] res					Return value from executed call.
	* @param[in] location				Description to be used for error reporting.
	*/
	void CheckResult(XI_RETURN res, const char* location);
private:
	xiProcessingHandle_t processing_handle_; //!< Handle for current processing context
	xiAPIplus_Image      last_image_;        //!< last image get by PullImage
};



// -----------------------------------------------
// class xiAPIplus - parameter enumerators
// -----------------------------------------------

class xiAPIplus_Enumerators
{
public:
	xiAPIplus_Enumerators();
	void Init();

	class param_item_t
	{
	public:
		param_item_t();
		param_item_t(const char * in_name, int in_value, const char * in_description);

		const char * name;
		int value;
		const char * description;
	
	};
	std::vector<param_item_t> XI_DOWNSAMPLING_VALUE_ITEMS;                            // Downsampling value enumerator.
	std::vector<param_item_t> XI_TEST_PATTERN_GENERATOR_ITEMS;                        // Test Pattern Generator
	std::vector<param_item_t> XI_VERSION_ITEMS;                                       // Module/Unit version selector
	std::vector<param_item_t> XI_TEST_PATTERN_ITEMS;                                  // Test Pattern Type
	std::vector<param_item_t> XI_DEC_PATTERN_ITEMS;                                   // Decimation Pattern Format
	std::vector<param_item_t> XI_BIN_PATTERN_ITEMS;                                   // Binning Pattern Format
	std::vector<param_item_t> XI_BIN_SELECTOR_ITEMS;                                  // Binning Engine Selector
	std::vector<param_item_t> XI_BIN_MODE_ITEMS;                                      // Selects binning mode; to be used with
	std::vector<param_item_t> XI_DEC_SELECTOR_ITEMS;                                  // Decimation Engine Selector
	std::vector<param_item_t> XI_SENSOR_TAP_CNT_ITEMS;                                // Sensor tap count enumerator.
	std::vector<param_item_t> XI_BIT_DEPTH_ITEMS;                                     // Bit depth enumerator.
	std::vector<param_item_t> XI_DEBUG_LEVEL_ITEMS;                                   // Debug level enumerator.
	std::vector<param_item_t> XI_IMG_FORMAT_ITEMS;                                    // Image output format enumerator.
	std::vector<param_item_t> XI_COLOR_FILTER_ARRAY_ITEMS;                            // Bayer color matrix enumerator.
	std::vector<param_item_t> XI_BP_ITEMS;                                            // structure containing information about buffer policy(can be safe, data will be copied to user/app buffer or unsafe, user will get internally allocated buffer without data copy).
	std::vector<param_item_t> XI_TRG_SOURCE_ITEMS;                                    // structure containing information about trigger source
	std::vector<param_item_t> XI_TRG_SELECTOR_ITEMS;                                  // structure containing information about trigger functionality
	std::vector<param_item_t> XI_TRG_OVERLAP_ITEMS;                                   // Trigger overlap modes
	std::vector<param_item_t> XI_ACQ_TIMING_MODE_ITEMS;                               // structure containing information about acquisition timing modes
	std::vector<param_item_t> XI_TRANSPORT_DATA_TARGET_MODE_ITEMS;                    // Enumerator for data target modes
	std::vector<param_item_t> XI_GPI_SEL_CB_ITEMS;                                    // Enumeration for XI_PRM_GPI_SELECTOR for CB cameras.
	std::vector<param_item_t> XI_GPO_SEL_CB_ITEMS;                                    // Enumeration for XI_PRM_GPO_SELECTOR for CB cameras.
	std::vector<param_item_t> XI_GPI_MODE_ITEMS;                                      // structure containing information about GPI functionality
	std::vector<param_item_t> XI_GPI_SELECTOR_ITEMS;                                  // Enumerator for GPI port selection.
	std::vector<param_item_t> XI_GPO_MODE_ITEMS;                                      // structure containing information about GPO functionality
	std::vector<param_item_t> XI_GPO_SELECTOR_ITEMS;                                  // Enumerator for GPO port selection.
	std::vector<param_item_t> XI_LED_MODE_ITEMS;                                      // structure containing information about LED functionality
	std::vector<param_item_t> XI_LED_SELECTOR_ITEMS;                                  // Enumerator for LED selection.
	std::vector<param_item_t> XI_COUNTER_SELECTOR_ITEMS;                              // structure contains frames counter
	std::vector<param_item_t> XI_TS_RST_MODE_ITEMS;                                   // structure containing information about timestamp reset arming
	std::vector<param_item_t> XI_TS_RST_SOURCE_ITEMS;                                 // structure containing information about possible timestamp reset sources
	std::vector<param_item_t> XI_PRM_TYPE_ITEMS;                                      // structure containing information about parameters type
	std::vector<param_item_t> XI_SWITCH_ITEMS;                                        // Turn parameter On/Off
	std::vector<param_item_t> XI_TEMP_SELECTOR_ITEMS;                                 // Temperature selector
	std::vector<param_item_t> XI_TEMP_CTRL_MODE_SELECTOR_ITEMS;                       // Temperature selector
	std::vector<param_item_t> XI_TEMP_ELEMENT_SELECTOR_ITEMS;                         // Temperature element selector
	std::vector<param_item_t> XI_OUTPUT_DATA_PACKING_TYPE_ITEMS;                      // Data packing(grouping) types.
	std::vector<param_item_t> XI_DOWNSAMPLING_TYPE_ITEMS;                             // Downsampling types
	std::vector<param_item_t> XI_EXPOSURE_TIME_SELECTOR_TYPE_ITEMS;                   // Exposure time selector
	std::vector<param_item_t> XI_INTERLINE_EXPOSURE_MODE_TYPE_ITEMS;                  // Interline exposure mode
	std::vector<param_item_t> XI_GAIN_SELECTOR_TYPE_ITEMS;                            // Gain selector
	std::vector<param_item_t> XI_SHUTTER_TYPE_ITEMS;                                  // Shutter mode types
	std::vector<param_item_t> XI_CMS_MODE_ITEMS;                                      // structure containing information about CMS functionality
	std::vector<param_item_t> XI_CMS_INTENT_ITEMS;                                    // structure containing information about ICC Intents
	std::vector<param_item_t> XI_OPEN_BY_ITEMS;                                       // structure containing information about options for selection of camera before opening
	std::vector<param_item_t> XI_LENS_FEATURE_ITEMS;                                  // Lens feature selector selects which feature will be accessed.
	std::vector<param_item_t> XI_SENSOR_FEATURE_SELECTOR_ITEMS;                       // Sensor feature selector selects which feature will be accessed.
	std::vector<param_item_t> XI_SENSOR_MODE_ITEMS;                                   // Camera sensor mode enumerator.
	std::vector<param_item_t> XI_IMAGE_AREA_SELECTOR_ITEMS;                           // Defines image sensor area as output.
	std::vector<param_item_t> XI_SENSOR_OUTPUT_CHANNEL_COUNT_ITEMS;                   // Camera channel count enumerator.
	std::vector<param_item_t> XI_SENS_DEFFECTS_CORR_LIST_SELECTOR_ITEMS;              // Sensor defects correction list selector
	std::vector<param_item_t> XI_ACQUISITION_STATUS_SELECTOR_ITEMS;                   // Acquisition status Selector
	std::vector<param_item_t> XI_DP_UNIT_SELECTOR_ITEMS;                              // Select unit where data-pipe is configured
	std::vector<param_item_t> XI_DP_PROC_SELECTOR_ITEMS;                              // Select unit processor
	std::vector<param_item_t> XI_DP_PARAM_SELECTOR_ITEMS;                             // Select processor parameter
	std::vector<param_item_t> XI_DP_PARAM_VALUE_ITEMS;                                // Select processor parameter value
	std::vector<param_item_t> XI_USER_SET_SELECTOR_ITEMS;                             // User Set selector options.
	std::vector<param_item_t> XI_DUAL_ADC_MODE_ITEMS;                                 // Mode of DualADC feature
	std::vector<param_item_t> XI_PROBE_SELECTOR_ITEMS;                                // Probe Selector
	std::vector<param_item_t> XI_TOF_READOUT_MODE_ITEMS;                              // ToF Readout Mode feature
	std::vector<param_item_t> XI_DATA_SM_ITEMS;                                       // Image data signedness

	std::vector<param_item_t>  XI_RET_ITEMS;
	static param_item_t * FindItem(std::vector<param_item_t> * param_vector, int item_val);	
};

class xiAPIplusCameraOcv : public xiAPIplus_Camera
{
public:
	/**
	* Initialize the xiAPIplusCameraOcv class.
	*/	
	xiAPIplusCameraOcv();
	/**
	* Class destructor. Free allocated memory, release images
	*/
	virtual ~xiAPIplusCameraOcv();
	/**
	* Reads an image from the camera using XiAPI, stores the image in OpenCV Mat format.
	* @return OpenCV Mat image. 
	*/
	cv::Mat			GetNextImageOcvMat(); //Reads an image and converts it to OpenCV Mat
	/**
	* Converts a XiAPI image (xiAPIplus_Image*) to OpenCV Mat.
	* @param input_image[in] Input xiAPIplus_Image* to be converted.
	* @return converted OpenCV IpliImage* image. 
	*/
	cv::Mat			ConvertOcvMat(xiAPIplus_Image * input_image); //Converts an image to OpenCV Mat
	//virtual xiAPIplusCameraOcv& operator >> (CV_OUT Mat& input_image);
    
    private:
	
	/**
	* Resets the Opencv image if properties of XI_IMG have changed. Resets the cv_mat_image_.
	*/
    void resetCvImage_();				//Resets the OpenCV image properties if XI_IMG format has changed
	cv::Mat 				cv_mat_image_;
	xiAPIplus_Image *	next_image_;      
	int					timeout_;
	int				counter_;
	int				index_;
};