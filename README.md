## Giới thiệu ##

- Đây chỉ làm một modules PAM thực hiện công việc khi người dùng ssh vào server thì tài khoản và mật khẩu của người dùng đó sẽ được chứng thực trên một con webservice đã dựng sẵn

### Built the PAM modules </br>

`gcc -fPIC -lcurl -c [ten_ma_nguon]`</br>
`ld -lcurl -x --shared -o /lib/security/[tenfile].so [Ten-file].o`</br>
- Trong trường hơp câu lệnh biên dịch không được do không thấy thư mục /lib/security , thì chúng ta hay tạo chúng ra
- Trong trường hợp khi chúng ta biên dịch bị lỗi thì chúng ta hãy dùng lệnh sau để update thêm thư viện cho service:</br>
`apt-get update`</br>
`apt-get install build-essential libpam0g-dev libcurl4-openssl-dev`</br>
- Khi biên dịch hay update nên sử dụng mức quyền siêu người dùng hoặc người dùng root
## Tạo một webservie
- Cài đặt máy chủ web trên một hệ điều hành mà bạn mong muốn, copy toàn bộ file trong thư mục webservie vào thư mục mà bạn đã tạo trang web.
- Tạo vhost cho web đó.
- Cuối cùng là check xem nó đã hoạt động chưa :D
## Cách sử dụng 
- Khi built xong chúng ra vào theo đường dẫn `/etc/pam.d/` và chỉnh sửa file `/etc/pam.d/sshd`. Sau đó chúng ta add thêm dòng:</br>
`auth       required     [ten-file-vua-built].so base_url=[Duong-dan-websereice]`</br>
- Lưu ý: xóa hết phần cấu hình cũ trong file sshd chỉ để lại một cấu hình do chúng ta vừa tạo
- Bây giờ thử ssh vào và tận hưởng thành quả nào :D
## Tài liệu tham khảo
- [Link 1](http://www.linux-pam.org/Linux-PAM-html/sag-overview.html)

- [Link 2](http://www.linux-pam.org/Linux-PAM-html/Linux-PAM_MWG.html)

- [Link 3](http://www.rkeene.org/projects/info/wiki/222)

- [Link 4](http://ben.akrin.com/?p=1068)

- [Link 5](https://github.com/beatgammit/simple-pam)
## Mô hình sequence diagram
<img src="https://drive.google.com/open?id=0B0cBCSMAtfSAOXJfVlJCRVNveU0">
