## Giới thiệu ##

- Đây chỉ làm một modules PAM thực hiện công việc khi người dùng ssh vào server thì tài khoản và mật khẩu của người dùng đó sẽ được chứng thực trên một con webservice đã dựng sẵn

### Built the PAM modules ###

`gcc -fPIC -lcurl -c [ten_ma_nguon]`
`ld -lcurl -x --shared -o /lib/security/[tenfile].so [Ten-file].o`

- Trong trường hợp khi chúng ta biên dịch bị lỗi thì chúng ta hãy dùng lệnh sau để update thêm thư viện cho service
`apt-get update`
`apt-get install build-essential libpam0g-dev libcurl4-openssl-dev`
- Khi biên dịch hay update nên sử dụng mức quyền siêu người dùng hoặc người dùng root
## Cách sử dụng ##
- Khi built xong chúng ra vào theo đường dẫn `/etc/pam.d/` và chỉnh sửa file `/etc/pam.d/sshd`. Sau đó chúng ta add thêm dòng :
`auth       required     [ten-file-vua-built].so base_url=[Duong-dan-websereice]`

- Bây giờ thử ssh vào và tận hưởng thành quả nào :D
