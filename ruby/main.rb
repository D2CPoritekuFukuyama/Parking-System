require "./NPlate_rcg.rb"
require "wiringpi"
require "open3"
require "./DAO/azure_LendingDAO.rb"
require "date"

def ticketing
	day = Time.now
	day_str = day.strftime("Now,%m/%d,%X")
	puts "券を発行します"
	Open3.capture3("../motor/gate_open")
	Open3.capture3("sudo ../printer/sample -------------------------------")
	Open3.capture3("sudo ../printer/sample Welcom!")
	Open3.capture3("sudo ../printer/sample '#{day_str}'")
	Open3.capture3("sudo ../printer/sample -------------------------------")
end

def record_inbound(id)
	lending_DAO = Lending_DAO.new()
	lending_DAO.record_adm(id)
end

nplate_RCG = Nplate_RCG.new()
is_gone = false #gateを通過したかどうか

io = WiringPi::GPIO.new do |gpio|
  gpio.pin_mode(0, WiringPi::INPUT)
  gpio.pin_mode(3, WiringPi::INPUT)
  #gpio.pin_mode(4, WiringPi::INPUT)
end
count = 0
loop do
	if io.digital_read(0) == 0 &&io.digital_read(4) == 0 && is_gone == false then
		puts "車が来ました"
		nplate_RCG.get_NumberPlate #ナンバー認識
		#会員かどうか
		if nplate_RCG.is_member == 1 then
			puts "会員様です"
			#ゲートオープン
			stdin, stdout, stderr = Open3.capture3("../motor/gate_open")
			#ナンバー情報表示
			nplate_RCG.nPlate.each do |num|	
	    		puts num
			end
			record_inbound(nplate_RCG.member_ID)
			#入り口ホールセンサがオフになったらtrueへ
			#センサー通過まで待機
			loop do

				if io.digital_read(0) == 1 then
					is_gone = true
					break							
				else
					if io.digital_read(4) == 1 then
						ticketing
						is_gone = true
						break
					end 
				end
			end	
		else
			count += 1
        	puts nplate_RCG.area
        	puts nplate_RCG.is_member
			if count > 5 #５回認証失敗で強制印字 
				ticketing
				Open3.capture3("../motor/gate_open")
				loop do
					if io.digital_read(0) == 1 then
						is_gone = true
						break
					end
				end
			end
		end
		
	elsif(io.digital_read(4) == 1 && count >0)
		record_inbound(nplate_RCG.member_ID)
		ticketing	
		is_gone = true
	elsif(io.digital_read(3) == 0 && is_gone == true)
		puts "車が出ました"
		stdin, stdout, stderr,thr_wait = Open3.capture3("../motor/gate_close")
		is_gone = false
		count = 0
	end
end