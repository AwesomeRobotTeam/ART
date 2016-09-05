#!/bash/bin

echo "Fan frequence [0-255] : $(cat /sys/kernel/debug/tegra_fan/target_pwm)"

echo "Usb autosuspend : $(cat /sys/module/usbcore/parameters/autosuspend)"

echo "Current CPU Clock Rate";

echo "CPU 0 : $(cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq)"
echo "CPU 1 : $(cat /sys/devices/system/cpu/cpu1/cpufreq/scaling_cur_freq)"
echo "CPU 2 : $(cat /sys/devices/system/cpu/cpu2/cpufreq/scaling_cur_freq)"
echo "CPU 3 : $(cat /sys/devices/system/cpu/cpu3/cpufreq/scaling_cur_freq)"

echo "CPU Quiet Enable : $(cat /sys/devices/system/cpu/cpuquiet/tegra_cpuquiet/enable)"


echo "CPU Online : $(cat /sys/devices/system/cpu/online)"

echo "CPU Scaling_governor : $(cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor)"

echo "Max EMC rate : $(cat /sys/kernel/debug/clock/emc/max)"
echo "Current EMC rate : $(cat /sys/kernel/debug/clock/override.emc/rate)"
echo "EMC state : $(cat /sys/kernel/debug/clock/override.emc/state)"
