# LIBMGR
Đồ án cuối kì môn Kỹ thuật Lập trình - Quản lý Thư viện - FIT@HCMUS
## Hướng dẫn sử dụng
> Tất cả các menu chọn đều sử dụng phím mũi tên lên/xuống/trái/phải để chuyển qua lại các chức năng/đối tượng

> Nhấn Enter để chọn chức năng/đối tượng đó

> Để quay lại menu trước, dùng phím ESC (kể cả trong lúc nhập liệu)

> Tất cả các trường thông tin đều không thể bị bỏ trống

> Bắt buộc phải đăng nhập để sử dụng chương trình

Quy định về ký tự được chấp nhận trong các trường thông tin:
- Mật khẩu: ``0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~! @#$%^&*_-+=`|\(){}[]:;'<>,.?/``

- Tên đăng nhập: `0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_-`

- Họ tên: `abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ `

- Email: `0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.@+`

- Số CMND và Ngày tháng năm: `0123456789`

- Địa chỉ: `0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ/_-:()., `


### Chức năng xác thực và quản lý tài khoản
#### Đăng nhập
Với lần đầu sử dụng, chương trình sẽ yêu cầu tạo một tài khoản admin mới. Người dùng cần nhập vào các thông tin cần thiết để tạo một tài khoản admin, rồi dùng tài khoản đó cho lần đầu đăng nhập.
Để sử dụng bất kì chức năng nào của chương trình đều yêu cầu đăng nhập tài khoản. Mỗi tài khoản có các quyền sử dụng các chức năng khác nhau, riêng tài khoản admin được thực hiện tất cả các chức năng.
Nếu nhập sai mật khẩu 3 lần, tài khoản sẽ bị khoá, chỉ có admin mới có quyền mở tài khoản
Riêng tài khoản admin nếu nhập sai 3 lần sẽ không bị khoá nhưng bị buộc dừng chương trình
#### Đăng xuất
Sau khi chọn đăng xuất trong menu quản lý tài khoản, người dùng sẽ được đưa về màn hình đăng nhập nếu vẫn muốn sử dụng tiếp các chức năng.
#### Thay đổi mật khẩu
1. Tài khoản admin được quyền thay đổi mật khẩu cho chính mình và cho người dùng khác: 
- Nếu thay đổi mật khẩu cho chính mình, người dùng cần nhập lại mật khẩu cũ và xác nhận mật khẩu mới.

- Nếu thay đổi mật khẩu cho tài khoản khác, người dùng admin chỉ cần nhập mật khẩu mới và xác nhận.

Sau khi chọn vào mục `Quản lý tài khoản > Thay đổi mật khẩu`, người dùng admin sẽ được đưa đến màn hình chọn tài khoản muốn cập nhật `Chính mình` hoặc `Người dùng khác`. Tại đây, nếu chọn `Chính mình`, admin cần nhập lại mật khẩu cũ và xác nhận mật khẩu mới để thay đổi mật khẩu. Nếu chọn `Người dùng khác`, nhập vào tên đăng nhập của tài khoản muốn thay đổi mật khẩu, sau khi chọn tài khoản, admin có quyền thay đổi mật khẩu của tài khoản đó.

2. Tài khoản quản lý hoặc chuyên viên chỉ được thay đổi mật khẩu của chính mình, khi đó người dùng cần nhập mật khẩu cũ và xác nhận mật khẩu mới, nếu nhập sai mật khẩu cũ 3 lần sẽ bị khoá tài khoản, chỉ có admin mới mở lại được tài khoản.
Sau khi chọn vào mục `Quản lý tài khoản > Thay đổi mật khẩu`, người dùng quản lý hoặc chuyên viên sẽ được đưa đến màn hình nhập mật khẩu cũ và xác nhận mật khẩu mới để thay đổi mật khẩu.
#### Cập nhật thông tin tài khoản
Chọn mục `Quản lý tài khoản > Cập nhật thông tin tài khoản`
Tên đăng nhập, CMND không thể bị trùng lặp. Nếu người dùng nhập vào trùng lặp sẽ có thông báo nhập lại.

Các bước thực hiện tương tự như thay đổi mật khẩu
1. Tài khoản admin được quyền cập nhật thông tin tài khoản cho chính mình và cho người dùng khác.

2. Tài khoản quản lý hoặc chuyên viên chỉ được cập nhật thông tin tài khoản của chính mình.
#### Tạo người dùng mới
>ADMIN ONLY

- Chọn mục `Quản lý tài khoản > Tạo người dùng` và nhập các thông tin cần thiết để tạo người dùng mới, dùng các thông tin này để đăng nhập vào tài khoản mới
- Tên đăng nhập, CMND không thể bị trùng lặp. Nếu người dùng nhập vào trùng lặp sẽ có thông báo nhập lại.
#### Phân quyền người dùng
>ADMIN ONLY

- Chọn mục `Quản lý tài khoản > Phân quyền người dùng`, nhập vào tên đăng nhập của tài khoản muốn phân quyền lại, sau khi chọn tài khoản, admin sẽ được chọn quyền mới muốn áp dụng lên tài khoản đã chọn.

### Chức năng quản lý độc giả
#### Xem danh sách thông tin độc giả
> Toàn bộ người dùng đều có quyền sử dụng chức năng này tại `Quản lý độc giả > Xem danh sách độc giả trong thư viện`

Sau khi chọn thì chương trình sẽ in ra toàn bộ thông tin của toàn bộ độc giả. Nhấn enter để quay trở lại menu
#### Thêm độc giả
> Toàn bộ người dùng đều có quyền sử dụng chức năng này tại `Quản lý độc giả > Thêm độc giả`

Chương trình sẽ yêu cầu người dùng nhập vào các thông tin cần thiết để thêm một độc giả mới, nếu thêm độc giả thành công sẽ có thông báo
#### Chỉnh sửa thông tin một độc giả
> Toàn bộ người dùng đều có quyền sử dụng chức năng này tại `Quản lý độc giả > Chỉnh sửa thông tin một độc giả`

Yêu cầu nhập vào các thông tin tương tự như thêm một độc giả mới, nếu thay đổi thành công sẽ có thông báo
#### Tìm kiếm độc giả theo số CMND
> Toàn bộ người dùng đều có quyền sử dụng chức năng này tại `Quản lý độc giả > Tìm kiếm độc giả theo CMND`

Yêu cầu nhập vào số CMND của độc giả cần tìm kiếm
- Nếu tìm được sẽ in ra toàn bộ thông tin của độc giả đó (nếu có thì độc giả đó là duy nhất do CMND không thể bị trùng lặp)
- Nếu không tìm được sẽ thông báo không có độc giả nào có số CMND đã nhập
#### Tìm kiếm sách theo tên độc giả
> Toàn bộ người dùng đều có quyền sử dụng chức năng này tại `Quản lý độc giả > Tìm kiếm sách theo họ tên độc giả`

Yêu cầu nhập vào tên độc giả, xuất ra danh sách những cuốn sách mà độc giả này đang mượn (nếu có)
#### Xoá thông tin một độc giả
> Chỉ có admin và quản lý có quyền sử dụng chức năng này tại `Quản lý độc giả > Xoá thông tin một độc giả`

Yêu cầu nhập vào số CMND của độc giả cần xoá thông tin
- Nếu độc giả tồn tại thì xoá thông tin của độc giả đó và thông báo xoá thành công
- Nếu không có độc giả nào có số CMND đã nhập thì không xoá, thông báo không tìm thấy độc giả
