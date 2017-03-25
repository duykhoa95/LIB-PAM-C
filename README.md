Cách sử dụng thư viện pam để đăng nhập không cần pass, và xác nhận qua một webservice or mail hay bất cứ thứ j
buiding mã nguôn thư viện PAM viết bằng c bằng câu lệnh
gcc -fPIC -lcurl -c [ten_ma_nguon]
ld -lcurl -x --shared -o /lib/security/2ndfactor.so 2ndfactor.o
nếu bị lỗi do thư viện thì nên dùng câu lênh sau để update thư viện
apt-get update
apt-get install build-essential libpam0g-dev libcurl4-openssl-dev
khi buiding xong thư viện thì bắt đầu bạn hook vào nơi bạn muốn thay đổi police cho chúng
ví dụ trong bài này tôi muốn thay đổi chính sách xác thực cho ssh
thì tôi vào theo đường dẫn /etc/pam.d/sshd , rồi tôi thêm
auth       required     2ndfactor.so base_url=http://my.server.com/send_
lúc đó ta đã sử dụng đc thư viện rồi
===============================================
cấu trúc của file
tất cả các cấu hình của của các dịch vụ đều lưu trong /etc/pam.d/* or /etc/pam_config
thư viện của c nêu muốn bổ sung trực tiếp tại /var/include/*


