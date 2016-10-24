require 'tiny_tds'  
require './DAO/azure_baseDAO.rb'

class Lending_DAO < Azure_DAO 
    def record_adm(id, adm_time)
        puts adm_time.to_s
        sql = "INSERT INTO 貸出(
                会員ID,
                入庫時間,
                出庫時間,
                売上
            )
            VALUES(
                #{id},
                '#{adm_time}',
                '#{adm_time}',
                0
            )"
        results = @client.execute(sql).do
    end
end

#test = Member_DAO.new
#test.get_member
