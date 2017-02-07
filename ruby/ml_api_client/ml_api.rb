require 'csv'
require 'json'
require 'net/http'
require "./ml_api_client/Http_Manager.rb"
require 'json'


class ML_api_client < Http_Manager
    attr_reader :body
    attr_reader :hiragana_list
    
    def csv_load
        result = Array.new
        File.foreach(file_name) do |str|
            result.push(str)
        end
        return result
    end
    private :csv_load

    def initialize(url, api_key)
        @hiragana_list = Array.new
        File.foreach('ml_api_client/hiragana_list.txt') do |str| 
            @hiragana_list.push(str)
        end

        super

        #input label の作成
        @colName = Array.new 
        784.times do |col|
            name = "Pixel" + col.to_s
            @colName.push(name)
        end
        @colName.push("label")
    end

    def set_value(file_name)
        #Hiraganaの作成
        csvData = CSV.read(file_name)
        colParams = Array.new #全Valuesの格納
        csvData.each do |data|
            pixel = data[0].to_i
            colParams.push(pixel.to_s)
        end 
        return colParams
    end
    private :set_value

    def get_data 
        hiragana_params = Array.new
        number_params = Array.new
		category_params = Array.new
		4.times do |count|			
        	number_params.push(set_value("Dataset/Number#{(count+1).to_s}.csv"))
		end
       hiragana_params.push(set_value('Dataset/Hiragana.csv'))
       hiragana_params.push(Array.new(785,"0"))
       category_params.push(set_value('Dataset/Number1.csv'))
       category_params.push(set_value('Dataset/Number2.csv'))
       category_params.push(set_value('Dataset/Number3.csv'))
        #p number_params
        @body = {            
            "Inputs" => {
                "Number" => {
                    "ColumnNames" => @colName,
                    "Values" => number_params,
                },
                "Category_Number" =>{
                    "ColumnNames" => @colName,
                    "Values" => category_params,
                },
                "Hiragana"=>{
                    "ColumnNames" => @colName,
                    "Values"=> hiragana_params,
                },
            },
            "GlobalParameters" => Hash.new(),
        }  
        @request.body = @body.to_json
#        puts @body.to_json
        @response = Net::HTTP.start(@uri.host,
                                   @uri.port, 
                                   "192.168.10.30",
                                   8080,
                                   :use_ssl => @uri.scheme =='https'
                                  )do |http|
            http.request(@request)    
        end
        json = @response.body
        results = JSON.parse(json)
        puts results
		File.unlink("Dataset/Number4.csv")
        result = results['Results']
        puts "json response: #{@hiragana_list[result[0][0].to_i]}"
        return result
#        @hiragana_list[result[0][0].to_i]
    end
end

#hiragana_api_client = ML_api_client.new('https://ussouthcentral.services.azureml.net/workspaces/17636602cf21485babb5f60e96be7642/services/c6e321b758e8497c94c5a6289da5a3bf/execute?api-version=2.0&details=true','HIRAGANA_ML_API_KEY' )
#results = hiragana_api_client.get_data
#p results
