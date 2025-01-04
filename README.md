## CYDtouchCalibration

First of all: **I wish everyone a Happy New Year**
 
$45^2 = 3^{2^2} \cdot 5^2 = \left(\sum\limits_{n=1}^9 n\right)^2 = \sum\limits_{n=1}^9 n^3 = {{10} \choose {2}}^2 = 2025$
---
The CYDtouchCalibration program shows how the touchpad of the CYD can be 
calibrated. The LovyanGFX and XPT2046_Bitbang libraries are used for this. The 
latter replaces the touch routines contained in LovyanGFX because these prevent 
the SD card from being used. Do not forget to comment out the corresponding 
section in the header file lgfx_ESP32_2432S028.h.

At the first start, the touchpad is not calibrated and the following screen is 
displayed:

![uncalibrated](/images/uncalibrated.png)

You can choose whether you want to calibrate the screen or use the programmed 
default values. The touchpad is calibrated after this step and the following 
screen is displayed:

![uncalibrated](/images/calibrated.png)

Now you can calibrate the touchpad again, delete the saved calibration data, 
delete the entire preferences in the NVS or continue with the main program.

If you select **Calibrate** or **Recalibrate**, the calibration points defined in the program are displayed one after the other, which you must tap 5 times to display the next calibration point or to complete the calibration. The taps are averaged to 
obtain a more accurate result.

![calibration](/images/calibration.png)

The two calibration points can be set as required. I have chosen them in such a way that the non-linearity of the touchpad is compensated to some extent. 

