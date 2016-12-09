require "wiringpi"
require "open3"

swith_state = 0
gate_state = 0
io = WiringPi::GPIO.new do |gpio|
  gpio.pin_mode(0, WiringPi::INPUT)
  gpio.pin_mode(3, WiringPi::INPUT)
  gpio.pin_mode(4, WiringPi::INPUT)
end
loop do
	switch_state = io.digital_read(4)
	puts switch_state
	if(switch_state == 1 && gate_state == 0) # Read from pin 1
		stdin, stdout, stderr = Open3.capture3("./motor/gate_open")
		gate_state = 1
	elsif(switch_state == 1 && gate_state == 1)
		stdin, stdout, stderr = Open3.capture3("./motor/gate_close")
		gate_state = 0
	end
end
