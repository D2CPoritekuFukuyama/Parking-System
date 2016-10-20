require "open3"
require "./ocr_Client_module.rb"
require "json"
include OCR_Recognize 


def parse_json(json_data)
    json =  JSON.parse(json_data) 
    regions = json["regions"]
    region = regions[0]
    lines = region["lines"]
    line = lines[0]
    words = line["words"]
    text = ""
    words.each do |word|
        text << word["text"]
    end
    result = ""
    text.chars do |str|
        if(str =~ /[^ -~｡-ﾟ]/)
            result << str
        end
    end
    #puts result 
    #puts stdArray[0]
    return result
end

begin
    stdin, stdout, stderr = Open3.capture3('../openCVTest1/main')
rescue => ex
    p stderr
    p ex
end
stdArray = stdin.split("\n")
puts parse_json(OCR_Recognize.get_AreaName())
puts stdArray[0]
