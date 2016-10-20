require 'tiny_tds'  
require './DAO/azure_baseDAO.rb'

class Member_DAO < Azure_DAO 
    def get_member
        sql = "SELECT * FROM 会員;"
        results = @client.execute(sql)
        results.each do |row|
            puts row
        end
    end
end

#test = Member_DAO.new
#test.get_member
