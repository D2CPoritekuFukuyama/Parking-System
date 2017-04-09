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

        #数字認識用input label の作成
        @num_col_name = Array.new 
        784.times do |col|
#            name =  col.to_s
            @num_col_name.push(col.to_s)
        end
        @num_col_name.push("label")

        #ひらがな認識用input label の作成
        @hiragana_col_name = Array.new 
        784.times do |col|
            name =  "Pixel" + col.to_s
            @hiragana_col_name.push(name)
        end
        @hiragana_col_name.push("label")
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
                    "ColumnNames" => @num_col_name,
                    "Values" => number_params,
                },
                "Category_Number" =>{
                    "ColumnNames" => @num_col_name,
                    "Values" => category_params,
                },
                "Hiragana"=>{
                    "ColumnNames" => @hiragana_col_name,
                    "Values"=> hiragana_params,
                },
            },
            "GlobalParameters" => Hash.new(),
        }  
        @request.body = @body.to_json
#        puts @body.to_json
        @response = Net::HTTP.start(@uri.host,
                                   @uri.port, 
                                   :use_ssl => @uri.scheme =='https'
                                  )do |http|
            http.request(@request)    
        end
        json = @response.body
        results = JSON.parse(json)
        puts results
		File.unlink("Dataset/Number4.csv")
#        result = results['Results']
#        @hiragana_list[result[0].to_i]
    end
end

#hiragana_api_client = ML_api_client.new('https://ussouthcentral.services.azureml.net/workspaces/17636602cf21485babb5f60e96be7642/services/c6e321b758e8497c94c5a6289da5a3bf/execute?api-version=2.0&details=true','HIRAGANA_ML_API_KEY' )
#
#puts hiragana_api_client.get_data["Number"]["value"]["Values"]
