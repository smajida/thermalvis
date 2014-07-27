/*! \file	flow.hpp
 *  \brief	Declarations for flow node configuration.
*/

#ifndef THERMALVIS_FLOW_CONFIG_H_
#define THERMALVIS_FLOW_CONFIG_H_

#define DEFAULT_SENSITIVITY 0.1

#define DEFAULT_MULTIPLIER_1 3
#define DEFAULT_MULTIPLIER_2 10

#define DEFAULT_ALPHA 	0.00
#define MS_PER_SEC 		1000.0

#define MINIMUM_GFTT_SENSITIVITY	0.001
#define MINIMUM_HARRIS_SENSITIVITY	0.00001
#define FAST_DETECTOR_SENSITIVITY_SCALING	40.0
#define GFTT_DETECTOR_SENSITIVITY_SCALING	0.1
#define HARRIS_DETECTOR_SENSITIVITY_SCALING	0.001

#define TIGHT_DISTANCE_CONSTRAINT 3
#define FEATURE_DROP_TRIGGER 0.40

#define MINIMUM_PROP_OF_PREVIOUS_FEATURES 0.8

#define SKIP_WARNING_TIME_PERIOD 10.0

// DEBUG:
// Red: 	Successfully tracked points
// Green:	Recovered points
// Purple:	Detected, unmatched points
// Yellow:	Detected and matched points
// Blue:	

#define COLOR_TRACKED_POINTS 		CV_RGB(255, 0, 0)		// Red
#define COLOR_TRACKED_PATH	 		CV_RGB(255, 128, 127)	// Light Red
#define COLOR_RECOVERED_POINTS 		CV_RGB(0, 255, 0)		// Green
#define COLOR_UNMATCHED_POINTS 		CV_RGB(255, 0, 255)		// Purple	
#define COLOR_MATCHED_POINTS 		CV_RGB(255, 255, 0)		// Yellow

#define ADAPTIVE_WINDOW_CONTINGENCY_FACTOR 3.0

#define MIN_FEATURES_FOR_FEATURE_MOTION	10
#define MAX_HISTORY_FRAMES	16

#define MATCHING_MODE_NN	0
#define MATCHING_MODE_NNDR	1
#define MATCHING_MODE_LDA	2

#define DETECTOR_OFF	0
#define DETECTOR_GFTT	1
#define DETECTOR_FAST	2
#define DETECTOR_HARRIS 3

/// \brief		Parameters that are shared between both real-time update configuration, and program launch configuration for flow
struct flowSharedData {
	int maxFeatures, minFeatures, drawingHistory, matchingMode;
	double  maxFrac, flowThreshold, minSeparation, maxVelocity, newFeaturesPeriod, delayTimeout;
	bool verboseMode, debugMode, showTrackHistory;
	bool adaptiveWindow, velocityPrediction, attemptHistoricalRecovery, autoTrackManagement, attemptMatching, detectEveryFrame;

	flowSharedData();
};

/// \brief		Parameters that are only changeable from real-time interface
struct flowRealtimeOnlyData {
	bool pauseMode;

	flowRealtimeOnlyData();
};

/// \brief		Parameters that are only changeable from launch interface
struct flowLaunchOnlyData {
	bool specialMode;
#ifndef _BUILD_FOR_ROS_
	bool displayDebug, displayGUI;
#endif
	flowLaunchOnlyData();
};

/// \brief		Substitute for ROS live configuration adjustments
class flowRealtimeData : public flowSharedData, public flowRealtimeOnlyData {
protected:
    // ...

public:
	flowRealtimeData() { }
};

#endif
