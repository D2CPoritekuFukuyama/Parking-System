require 'tiny_tds'  
require './DAO/azure_baseDAO.rb'

class Member_DAO < Azure_DAO 
    def get_member(number, cate_num, hiragana, area)
        sql = "SELECT 会員ID,PLATENo1, PLATENo2, Area, hiragana 
                FROM 会員 
                WHERE PLATENo1 = '#{cate_num.to_i}'
                AND PLATENo2 = '#{number.to_i}'
                AND Area = (N'#{area.sub(/\s/,"")}');"

#                AND hiragana = 'hiragana';" 
		puts sql
        results = @client.execute(sql)
#        results.each do |row|
#            puts row
#        end
        return results
    end
end

