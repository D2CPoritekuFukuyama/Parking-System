require 'csv'
require 'json'
require 'net/http'
require "./Http_Manager.rb"
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
        File.foreach('hiragana_list.txt') do |str|            
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
        hiragana_params.push(set_value('num_test2.csv'))
        hiragana_params.push(Array.new(785,"0"))
        number_params.push(set_value('num_test2.csv'))
        number_params.push(set_value('num_test4.csv'))
        number_params.push(set_value('num_test4.csv'))
        p number_params
        @body = {            
            "Inputs" => {
                "Hiragana" => {
                    "ColumnNames" => @colName,
                    "Values" => hiragana_params,
                },
                "Number" =>{
                    "ColumnNames" => @colName,
                    "Values" => number_params,
                },
                "Category_Number"=>{
                    "ColumnNames" => @colName,
                    "Values"=> number_params,
                },
            },
            "GlobalParameters" => Hash.new(),
        }  
        @request.body = @body.to_json
        #puts @body.to_json
        @response = Net::HTTP.start(@uri.host,
                                   @uri.port, 
                                   :use_ssl => @uri.scheme =='https'
                                  )do |http|
            http.request(@request)    
        end
        json = @response.body
        results = JSON.parse(json)
        puts results
        #result = results['Results']['output1']['value']['Values'][0]
        #@hiragana_list[result[0].to_i]
    end
end

hiragana_api_client = ML_api_client.new('https://ussouthcentral.services.azureml.net/workspaces/17636602cf21485babb5f60e96be7642/services/c6e321b758e8497c94c5a6289da5a3bf/execute?api-version=2.0&details=true','HIRAGANA_ML_API_KEY' )
hiragana_api_client.get_data
