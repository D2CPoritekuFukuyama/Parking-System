require 'tiny_tds'  

class Azure_DAO
    attr_reader :clinet
    def initialize()
        @client = TinyTds::Client.new( 
            username: "#{ENV['SQL_DATABASE_USERID']}@#{ENV['SQL_DATABASE_SERVER']}.database.windows.net", 
            password: ENV['SQL_DATABASE_PASSWORD'],  
            host: "#{ENV['SQL_DATABASE_SERVER']}.database.windows.net", port: 1433,  
            database: 'ParkingSystem', azure: true)
    end

    def get_table()
        sql = "SELECT name FROM sysobjects WHERE xtype = 'U';"
        results = @client.execute(sql)
        results.each do |row|
            puts row 
        end 
 
    end
end
