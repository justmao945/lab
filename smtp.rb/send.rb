require 'net/smtp'

from = "test@test.com"
to = "justmao945@gmail.com"
to1 = "jianjun@atrenta.com"
cc = "327118385@qq.com"

to, cc = cc, to

msg = <<EOF
From: demo <#{from}>
To: Jianjun Mao <#{to}>
To: Mao Jianjun <#{to1}>
Cc: Jianjun Mao <#{cc}>
Subject: SMTP.rb test
Date: #{Time.now}
 

HaHa, this mail is sent by ruby.
EOF

Net::SMTP.start('localhost') do |smtp|
    smtp.send_message msg, from, [to,to1,cc]
end

