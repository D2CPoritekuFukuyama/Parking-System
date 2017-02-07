require "open3"
require "./ocr_Client_module.rb"
require "json"
require "./DAO/azure_MemberDAO.rb"
require "./ml_api_client/ml_api.rb"
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
			number = String.new
			category = String.new
			@nPlate = Array.new()
            stdin, stdout, stderr = Open3.capture3('../Nplate_rcg/main')
			#画像処理後Datasetのcsvファイルパラメートとしてml_apiへ投げる
			hiragana_api_client = ML_api_client.new('https://ussouthcentral.services.azureml.net/workspaces/17636602cf21485babb5f60e96be7642/services/c6e321b758e8497c94c5a6289da5a3bf/execute?api-version=2.0&details=true','HIRAGANA_ML_API_KEY' )
			results = hiragana_api_client.get_data
			#ナンバーの取得
			results["Number"]["value"]["Values"].each do |n|
				number << n[0]
			end
			@nPlate.push(number)
			#分類番号の取得
			results["Category_number"]["value"]["Values"].each do |cn|
				category << cn[0]
			end
			@nPlate.push(category)
			#ひらがなの取得
			@nPlate.push(hiragana_api_client.hiragana_list[results["Hiragana"]["value"]["Values"][0][0].to_i])
			p @nPlate

        rescue => ex
            p stderr
            p ex
        end
    end
    private :fetch_car_number

    def fetch_area_name
        @area = OCR_Recognize.parse_json(OCR_Recognize.get_AreaName())
        puts @area
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
		puts @area
        if (nPlate[2] != 0 && area != nil)
            result = @memberDAO.get_member(@nPlate[0], @nPlate[1], @nPlate[2], area)
			puts result
            if (result.count == 1 )
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
