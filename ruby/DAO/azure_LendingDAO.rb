require 'tiny_tds'  
require './DAO/azure_baseDAO.rb'
require 'date'

class Lending_DAO < Azure_DAO 
	def initialize
		super
		now_time = DateTime.now
		@str_admDate = now_time.strftime("%Y-%m-%d %H-%M-00")
		
	end

    def record_adm(id)
        sql = "INSERT INTO 貸出(
                会員ID,
                入庫時間,
                出庫時間,
                売上
            )
            VALUES(
                #{id},
                '#{@str_admDate}',
                '#{@str_admDate}',
                0
            )"
        results = @client.execute(sql)
		puts "登録完了"
    end

	def record_member_leaving(id)
       sql = "UPDATE 貸出
	   		 SET 出庫時間 = #{@str_admDate},
	   		 WHERE 会員ID = #{id}
			 AND 入庫時間 = 出庫時間"
       results = @client.execute(sql)end

end

#test = Member_DAO.new
#test.get_member
