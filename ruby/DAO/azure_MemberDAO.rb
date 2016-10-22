require 'tiny_tds'  
require './DAO/azure_baseDAO.rb'

class Member_DAO < Azure_DAO 
    def get_member(number, cate_num, hiragana, area)
#        sql = "SELECT COUNT(PLATENo1), COUNT(PLATENo2), COUNT(Area), COUNT(hiragana) 
#                FROM 会員
#                WHRE PLATENo2 = '#{number[0] + number[1] + '-' + number[2] + number[3]}'
#                AND PLATENo1 = '300'
#                AND Area = (N'#{area.sub(/\s/,'')}')
#                AND hiragana = 'hiragana';"
        sql = "SELECT PLATENo1, PLATENo2, Area, hiragana 
                FROM 会員 
                WHERE PLATENo1 = '#{cate_num.to_i}'
                AND PLATENo2 = '#{number}';"
#                AND Area = (N'#{area.sub(/\s/,'')}')
#                AND hiragana = 'hiragana';" 
        results = @client.execute(sql)
        puts results.count 
#        results.each do |row|
#            puts row
#        end
    end
end

#test = Member_DAO.new
#test.get_member
