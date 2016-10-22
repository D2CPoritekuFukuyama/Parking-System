require "open3"
require "./ocr_Client_module.rb"
require "json"
require "./DAO/azure_MemberDAO.rb"
include OCR_Recognize 

memberDAO = Member_DAO.new 
#memberDAO.get_member
while(true)
    begin
        stdin, stdout, stderr = Open3.capture3('../openCVTest1/main')
    rescue => ex
        p stderr
        p ex
    end
    stdArray = stdin.split("\n")
    nPlate = stdArray[0].split(" ")
    area = OCR_Recognize.parse_json(OCR_Recognize.get_AreaName())
    File.foreach('Area.text') do |a|
        if area =~ /[*#{area}*]/
            area = a
            break
        end
    end
    if (nPlate[2] != 0 && area != nil)
        puts nPlate
        puts area
        memberDAO.get_member(nPlate[1], nPlate[2], nPlate[0], area)
        break 
    end
end
