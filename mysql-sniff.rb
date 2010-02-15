#! /usr/bin/env ruby

$: << '/ops/lib/ruby'

require 'pcaplet'

MYSQL_COM_QUERY = 0x03

mysqlsniff = Pcaplet.new('-s 65535 -i eth0')
mysqlsniff.add_filter Pcap::Filter.new('tcp port 3306', mysqlsniff.capture)

query = ''
query_length = 0

mysqlsniff.each_packet do |pkt|
  next unless pkt.tcp_data
  if query then
    if query.length == query_length then
      puts query
      puts '-' * 80
      query = nil
      query_length = 0
    else
      query << pkt.tcp_data
    end
  end

  if pkt.tcp_data[3] == 0 and pkt.tcp_data[4] == MYSQL_COM_QUERY then
    query_length = (pkt.tcp_data[0,3] + "\0").unpack('V')[0] - 1
    next if query_length < 1
    query = pkt.tcp_data[5..-1]
  end
end
