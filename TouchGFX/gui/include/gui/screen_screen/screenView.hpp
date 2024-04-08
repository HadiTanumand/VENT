#ifndef SCREENVIEW_HPP
#define SCREENVIEW_HPP
#include <gui_generated/screen_screen/screenViewBase.hpp>
#include <gui/screen_screen/screenPresenter.hpp>
#include <touchgfx/Texts.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include "main.h"
#include "quadspi.h"
#include "W25Q256FV_QSPI.h"
#include "stm32h7xx_hal.h"
#include "stm32h7xx_it.h"
#include <stdio.h>

class screenView : public screenViewBase
{
public:
	screenView();
	virtual ~screenView() {}
	virtual void setupScreen();
	virtual void tearDownScreen();
	virtual void handleTickEvent();
	void handleInitAgeScreen();
	void handleInitCircuitScreen();
	void handleInitTestCalibrationScreen();
	void handleSensorsScreen();
	void handleTightnessScreen();
	void handleTightnessTestScreen();
	void handleFlowSensorScreen();
	void handleFlowSensor_backwardTestScreen();
	void handleFlowSensor_forwardScreen();
	void handleFlowSensor_forwardTestScreen();
	void handleMasksScreen();
	void handleInterfaceScreen();
	virtual void startVent();
	virtual void pauseVent();
	virtual void load_data();
	virtual void save_ventHours();
	virtual void save_ventMinutes();
	virtual void click_Monitoring();
	virtual void click_adult_initScreen_button();
	virtual void click_pediatric_initScreen_button();
	virtual void click_singleLeak_button();
	virtual void click_singleCircuit_button();
	virtual void click_testCalibration_button();
	virtual void click_sensors_button();
	virtual void click_skip_button();
	virtual void click_tightness_button();
	virtual void click_flowSensor_button();
	virtual void click_CO2Sensor_button();
	virtual void click_O2Cell_button();
	virtual void click_tightness_continue_button();
	virtual void click_flowSensor_continue_button();
	virtual void click_O2Check_button();
	virtual void click_CO2Check_button();
	virtual void click_SPO2Check_button();
	virtual void click_backInitialize();
	virtual void click_mask();
	virtual void click_invasive();
	virtual void click_ventedMask();
	virtual void click_nonVentedMask();
	virtual void click_setting();
	virtual void click_PEEP();
	virtual void click_PEEP_conf();
	virtual void click_PControl();
	virtual void click_PControl_conf();
	virtual void click_PControlMax_conf();
	virtual void click_IPAP_conf();
	virtual void click_IPAPMax_conf();
	virtual void click_RR();
	virtual void click_RR_conf();
	virtual void click_VT();
	virtual void click_VT_conf();
	virtual void click_IE();
	virtual void click_safetyVT_conf();
	virtual void click_FiO2();
	virtual void click_FiO2_conf();
	virtual void click_FiO2Alarm();
	virtual void click_Ti();
	virtual void click_Ti_conf();
	virtual void click_TiMax_ventSetting();
	virtual void click_TiMin_ventSetting();
	virtual void click_Psupp();
	virtual void click_Psupp_conf();
	virtual void click_PSMax_conf();
	virtual void click_sensitivity();
	virtual void click_sensitivity_conf();
	virtual void click_trigger();
	virtual void click_trigger_conf();
	virtual void click_flow();
	virtual void click_TiInspPause_conf();
	virtual void click_flowShape_conf();
	virtual void click_riseTime();
	virtual void click_riseTime_conf();
	virtual void click_cycling();
	virtual void click_cycling_conf();
	virtual void click_CPAP();
	virtual void click_CPAP_conf();
	virtual void click_EPAP_conf();
	virtual void click_confirm();
	virtual void click_backSetting();
	virtual void click_mode();
	virtual void click_patientInfo();
	virtual void click_heightButton();
	virtual void click_age();
	virtual void click_adult();
	virtual void click_pediatric();
	virtual void click_ventSetting();
	virtual void click_trends();
	virtual void click_alarmRecords();
	virtual void click_dataRecords();
	virtual void click_next();
	virtual void click_next2x();
	virtual void click_previous();
	virtual void click_previous2x();
	virtual void click_device();
	virtual void click_deviceInfo();
	virtual void click_circuitSetting();
	virtual void click_backupSetting();
	virtual void click_backupModes();
	virtual void click_backBackupModeScreen();
	virtual void click_backupConfigs();
	virtual void click_apneaDetection();
	virtual void click_backupApnea();
	virtual void click_alarm();
	virtual void click_activeAlarms();
	virtual void click_backActiveAlarms();
	virtual void click_backVentSetting();
	virtual void click_backTrendsScreen();
	virtual void click_backPatientInfo();
	virtual void click_backBackupScreen();
	virtual void click_PEEPBackupScreen();
	virtual void click_VTBackupScreen();
	virtual void click_RRBackupScreen();
	virtual void click_TiBackupScreen();
	virtual void click_flowShapeBackupScreen();
	virtual void click_riseTimeBackupScreen();
	virtual void click_PControlBackupScreen();
	virtual void click_backDeviceS();
	virtual void click_backDeviceInfo();
	virtual void click_audio();
	virtual void click_audioSlider(int val);
	virtual void click_brightness();
	virtual void click_brightnessSlider(int val);
	virtual void click_time();
	virtual void click_date();
	virtual void click_language();
	virtual void click_english();
	virtual void click_arabic();
	virtual void click_backAlarmSc();
	virtual void click_RRAlarmbtn();
	virtual void click_PAlarmbtn();
	virtual void click_VtiAlarmbtn();	
	virtual void click_VteAlarmbtn();
	virtual void click_MViAlarmbtn();
	virtual void click_MVeAlarmbtn();
	virtual void click_lowPEEP();
	virtual void click_leakAlarm();
	virtual void click_SPO2Alarm();
	virtual void click_toggleApnea();
	virtual void click_backApneaAlarm();
	virtual void click_circuitDisAlarm();
	virtual void click_pulsRateAlarm();
	virtual void click_ventStopedAlarm();
	virtual void click_FiO2FakeAlarm();
	virtual void click_obstructionAlarm();
	virtual void click_autoscaleButton();
	virtual void click_configurationButton();
	virtual void click_dataButton1();
	virtual void click_dataButton2();
	virtual void click_dataButton3();
	virtual void click_dataButton4();
	virtual void click_backDataGraphs();
	virtual void click_nextPageAlarm();
	virtual void click_previousPageAlarm();
	virtual void click_back_flowSensorScreen();
	virtual void click_back_tightnessScreen();

	void handleMainScreen();
	void disableBackupModeChooses();
	void updateDate();
	void handleVentilationSettingScreen();
	void handlePatientInfoSettingScreen();
	void handleSettingScreen();
	void handleChooseMode();
	void handleAudio();
	void handleBrightness();
	void handleAge();
	void handleDay();
	void handleMonth();
	void handleYear();
	void handleWeek();
	void handleHours();
	void handleMinutes();
	void handleDeviceSetting();
	void handleDeviceInfo();
	void handleAlarm();
	void handleApneaDetection();
	void handleAlarmNextPage();
	void handleCircuitDisAlarm();
	void handleHRMaxAlarm();
	void handleHRMinAlarm();
	void handleObstructionAlarm();
	void handleVentStopedAlarm();
	void handleFiO2FakeAlarm();
	void handleTrends();
	void handleLanguage();
	void handlePEEP();
	void handlePIP();
	void handleRR();
	void handleVT();
	void handleIE();
	void handleFiO2();
	void handleTi();
	void handlePsupp();
	void handleCPAP();
	void handleFlow();
	void handleSens();
	void handleRisetime();
	void handleTrigger();
	void handleCycling();
	void handlePEEP_CONF();
	void handlePControl_CONF();
	void handlePControlMax_CONF();
	void handleEPAP_CONF();
	void handleIPAP_CONF();
	void handleIPAPMax_CONF();
	void handleRR_CONF();
	void handleVT_CONF();
	void handleSafetyVT_CONF();
	void handleFiO2_CONF();
	void handleTi_CONF();
	void handleTiMax_CONF();
	void handleTiMin_CONF();
	void handlePsupp_CONF();
	void handlePSMax_CONF();
	void handleTiInspPause_CONF();
	void handleFlowShape_CONF();
	void handleSens_CONF();
	void handleRisetime_CONF();
	void handleTrigger_CONF();
	void handleCycling_CONF();
	void handleCPAP_CONF();
	void handleRRMax();
	void handleRRMin();
	void handleWeight();
	void handlePressureMax();
	void handlePressureMin();
	void handleVtiMax();
	void handleVtiMin();
	void handleVteMax();
	void handleVteMin();
	void handleMViMax();
	void handleMViMin();
	void handleMVeMax();
	void handleMVeMin();
	void handleFiO2Max();
	void handleFiO2Min();
	void handleSPO2Max();
	void handleSPO2Min();
	void handleLowPEEP();
	void handleLeakMax();
	void handleBackup();
	void handleBackupChooseModes();
	void handleBackupApneaTime();
	void handleBackupConfigScreen();
	void handlePEEPBackup();
	void handleRRBackup();
	void handleVTBackup();
	void handleTiBackup();
	void handlePControlBackup();
	void handleRiseTimeBackup();
	void handleFlowShapeBackup();
	void handleActiveAlarams();
	void handleApneaAlaram();
	void handleAlarmShowing();
	void disableMainChoose();
	void sendConfig();
	void disableVentSettingChoose();
	void disableVentSettingChoose2();
	uint32_t findNextEmptyQSPI(uint32_t startPoint, uint32_t point, uint32_t endPoint);
	void save_trendAlarm(uint8_t str[]);
	void load_trendAlarms();
	void load_startAlarmPointer();
	void save_startAlarmPointer();
	void createAlarmTrendList();
	void createDataTrendList();
	void load_startDataPointer();
	void save_startDataPointer();
	void load_trendData_last4n();
	void save_trendData();
	void save_calibrationDate(uint8_t state);
	void load_calibrationData();
	void setCalibrationData();
	char* convertIdToMode(uint8_t val);
	void handleAlarmAudioLED();
	void handleSilent();
	void updateIdealWeightParameter();
	void showAlert();
	void handleAlert();
	void updateConfigsWildCards();
	virtual void click_yesAlert();
	virtual void click_cancelAlert();	
	void setLanguage();
	
protected:
	int tickCounter;
	uint8_t h;
	uint8_t m;
	
	float mode_int;
	float mode_tmp_int; // between 1 and .. => 1:CMV-VCV, 2:CMV-PCV, 3:AC-VCV, 4:AC-PCV, 5:SIMV_VCV, 6:SIMV_PCV, 7:PSV, 8:CPAP, 9:BiPAP
	char date[11];
	
	char trendDates[40][16];
	uint32_t TrendCurrAddr; // current address of the QSPI
	short int trendBuffCounter; // number of times buffer should send data to QSPI
	uint8_t patientNum; // 1 to 40
	uint8_t patientNumShowTrend; // 1 to 40
	uint32_t trendSizes[40]; //store size of each trend file
	
	uint8_t monitorState; // 0=> waves, 1=> .., 2=> ..
	enum State {INIT_AGE, INIT_CIRCUIT, TESTCALIBRATION, SENSORS, TIGHTNESS, TIGHTNESS_TEST, FLOWSENSOR, FLOWSENSOR_BACKWARDTEST, FLOWSENSOR_FORWARD,FLOWSENSOR_FORWARDTEST, O2CELL_TEST, CO2SENSOR_TEST, MAIN, VENT_SETTING, PATIENT_INFO, NEW_PATIENT, TRENDS_SCREEN, BACKUP, BACKUP_MODE, BACKUP_MODESCREEN,
		BACKUP_APNEA,	SETTING, PEEP, PEEP_CONF, PIP, PIP_CONF, PCONTROL_CONF, PCONTROLMAX_CONF, RR, RR_CONF, VT,	VT_CONF, IE, IE_CONF, FIO2, FIO2_MIN, FIO2_MAX, LEAK_MAX, LOWPEEP,
		FIO2_CONF, TI, TI_CONF, TIMAX_CONF, TIMIN_CONF, FLOW, TIINSPPAUSE_CONF, SENS, SENS_CONF, RISETIME, RISETIME_CONF,PSUPP, PSUPP_CONF, PSMAX_CONF, TRIGGER, FLOWSHAPE, FLOWSHAPE_CONF,
		TRIGGER_CONF, CPAP, CPAP_CONF, SVT_CONF, CYCLING_OFF,	CYCLING_OFF_CONF, MODE, AGE, WEIGHT, AUDIO, BRIGHTNESS, DEVICE, DEVICEINFO, DAY, MONTH,
		YEAR, WEEK, MINUTES, HOURS, ALARM, LANGUAGE, RR_MAX, RR_MIN, PRESSURE_MAX, PRESSURE_MIN,	VTI_MAX, VTE_MAX,	VTI_MIN, VTE_MIN, MVI_MAX, MVE_MAX,
		MVI_MIN, MVE_MIN, FIO2_ALARM, APNEA_ALARM, APNEA_ALARM_VAL, MASK_OR_NOT, MASKS, BACKUP_PEEP, BACKUP_TI, BACKUP_VT, BACKUP_FLOWSHAPE,
		BACKUP_PCONTROL, BACKUP_RR, BACKUP_RISETIME, ACTIVE_ALARMS, SPO2_MIN, SPO2_MAX, ALARM_NEXTPAGE, CIRCUIT_ALARM, PULSERATE_MAX, PULSERATE_MIN,
		OBSTRUCTION_ALARM, VENTSTOPED_ALARM, IPAPMAX_CONF, IPAP_CONF, EPAP_CONF, APNEA_DETECTION, ALERT
	};
	State state;
	State lastState;
	
	enum Alarms {
		NONE=0, LOW_P_H=4, OBSTRUCTION=5, HIGH_P_H=6, HIGH_P_M=41, HIGH_RR_H=17, LOW_RR_H=16, HIGH_MVI_H=10, LOW_MVI_H=9, HIGH_MVE_H=11, LOW_MVE_H=8, HIGH_VTI_H=15,
		HIGH_VTE_H=13, CRITIC_LOWBAT_H=26, BATT_INOPERABLE_H=29, HIGH_PEEP_M=43, LOW_PEEP_M=42
	};
	Alarms alarms;
	
	enum LastButton {
		STOPVENT_BUTTON = 1, LOCK_BUTTON = 2, UNLOCK_BUTTON = 3, WEIGHT_BUTTON = 4, CIRCUITSETTING_BUTTON = 5, CPR_BUTTON = 6
	};
	LastButton lastButton;
	
};

#endif // SCREENVIEW_HPP
