require "open3"
require "./ocr_Client_module.rb"
require "json"
require "./DAO/azure_MemberDAO.rb"
include OCR_Recognize 

class Nplate_RCG
    attr_reader :nPlate
    attr_reader :area
    attr_reader :is_member
	attr_reader :member_ID
    
    def initialize()
        @memberDAO = Member_DAO.new 
    end

    def fetch_car_number
        begin
            stdin, stdout, stderr = Open3.capture3('../openCVtest1/main')
        rescue => ex
            p stderr
            p ex
        end
        stdArray = stdin.split("\n")
        @nPlate = stdArray[0].split(" ")
    end
    private :fetch_car_number

    def fetch_area_name
        @area = OCR_Recognize.parse_json(OCR_Recognize.get_AreaName())
        #puts @area
        File.foreach('Area.text') do |a|
            if a =~ /[*#{area}*]/
                @area = a
                return @area
            end
        end
        @area ="" 
    end 
    private :fetch_area_name

    def get_NumberPlate
        fetch_car_number
        fetch_area_name
        if (nPlate[2] != 0 && area != nil)
            #puts nPlate
            #puts area
            result = @memberDAO.get_member(nPlate[1], nPlate[2], nPlate[0], area)
            if result.count == 1 
                @is_member = 1
				result.each do |row|
					@member_ID = row[0]
				end
            else
                @is_member = 0
            end
        end
    end
    public :get_NumberPlate
end
