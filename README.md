# Board-game
## Hướng dẫn cài đặt

Bước 1: Tải và cài đặt CodeBlocks tại [link](http://www.codeblocks.org/downloads/binaries/) (chọn phiên bản phù hợp với hệ điều hành)

Bước 2: Tải code trên GitHub về rồi giải nén, sau đó chạy file Board-game.cbp

Bước 3: Sau khi mở được project trên CodeBlocks thì vào Build -> Build and run (hoặc ấn F9) để có thể chạy chương trình. *Note: SDL2 đã có sẵn trong source code, không cần link thêm*
  
**Lưu ý:** Hiện tại game chưa hỗ trợ chạy trực tiếp bằng file `.exe` hoặc build bằng `Makefile`. Những tính năng đó sẽ được thêm trong các phiên bản sau. 

## Mô tả chung về trò chơi

Game bao gồm 3 trò chơi là Cờ vua (Chess), Cờ đam (Checker) và Othello (Reversi). Người chơi chọn 1 trò chơi trên màn hình chính để bắt đầu. *Note: Hiện tại chỉ 'Cờ vua' có thể chơi được, các game khác sẽ có trong các phiên bản sau*
  
### 1. Cờ vua

2 người chơi lần lượt di chuyển các quân cờ với mục đích chiếu hết vua đối phương

Luật chơi, các nước đi có thể, trạng thái kết thúc ván cờ có thể xem thêm [tại đây](https://www.chess.com/vi/cach-choi-co-vua)

## Mô tả các chức năng đã cài đặt

- Kéo thả các quân cờ để di chuyển chúng. Khi kéo 1 quân cờ, các vị trí quân cờ đó có thể đi tới sẽ được highlight
- Vua đang bị chiếu sẽ thể hiện trên màn hinh. Không thể di chuyển vua vào ô đang bị tấn công, hoặc di chuyển 1 quân mà khiến cho vua bị chiếu
- Các lựa chọn phong cấp thể hiện phía bên phải bàn cờ
- Nhấn phím Esc hoặc click vào nút X để quay lại màn hình chính
- Nhấn phím R hoặc click vào nút :arrows_counterclockwise: để chơi lại từ đầu
- Nhấn phím Z hoặc click vào nút :leftwards_arrow_with_hook: để quay lại 1 nước trước đó (không khả dụng khi ván cờ đã kết thúc)
- Các tính năng đặc biệt dùng cho debug
  - Nhấn phím Q để đi 1 nước ngẫu nhiên
  - Nhấn phím C để xuất ra (trên màn hình console) số trạng thái bàn cờ có thể xảy ra ([Perft result](https://www.chessprogramming.org/Perft_Results)) sau n nước đi (có thể thay đỏi trong chương trình)

## Các kĩ thuật lập trình được sử dụng trong chương trình

- Bàn cờ được lưu trữ theo hướng *Square Centric*, sử dụng 1 số nguyên 8 bit để lưu quân cờ trên 1 ô (với 6 bit thấp dùng để xác định loại quân cờ, 2 bit cao dùng để xác định màu của quân cờ)
- Trạng thái bàn cờ vua được lưu trữ dưới dạng [Forsyth-Edwards Notation](https://www.chessprogramming.org/Forsyth-Edwards_Notation)
- Các nước đi của cờ vua được lưu dưới dạng [Extended Position Description](https://www.chessprogramming.org/Extended_Position_Description)
- Cố định framerate của game bằng `SDL_Delay` để giảm tốc độ xử lý và hạn chế phải render nhiều lần, tránh tình trạng quá tải bộ nhớ và CPU
- Lập trình hướng đối tượng sử dụng `Class` và `Struct`
- Đồ hoạ sử dụng SDL và SDL_image
- Sử dụng các container: Vector (lưu vị trí quân cờ, các nước đi có thể...), Stack (lưu nước đi đã thực hiện)
- Sử dụng con trỏ và cấp phát động bộ nhớ hợp lý, hạn chế tình trạng tràn dữ liệu
- Dùng một số tính năng cơ bản của C++ như vòng lặp, các hàm có sẵn của thư viện...

## Hướng phát triển
1. Các chức năng còn thiếu
- Cờ đam và Othello
- Hoà cờ khi 1 thế cờ lặp lại 3 lần

2. Các tính năng bổ sung
- AI cho tính năng 1 người chơi
- Tính thời gian cho ván cờ
- Cho phép xin hoà hoặc xin thua
- Ghi lại và đánh giá các nước đi
- Xuất kết quả ra file

3. Code cần sửa đổi và hoàn thiện
- Sửa lỗi thỉnh thoảng không thể di chuyển quân để chắn cho vua đang bị chiếu
- Sử dụng các thuật toán đẩy nhanh tốc độ chương trình (VD dùng Tree để lưu trạng thái bàn cờ...)
- Refactor & Optimize code

## Các điều tâm đắc rút ra được sau khi hoàn thiện chương trình
- Cách khai báo, lưu trữ và truy cập các đối tượng trong Class
- Cách tách file, cách quản lí project nhiều file
- Cách code sạch, refactor chương trình, thêm comment

## Nguồn tham khảo

Ý tưởng, hướng thiết kế: https://www.youtube.com/watch?v=kwQk5A26cis

Xử lý đò hoạ: [Lazyfoo](https://lazyfoo.net/tutorials/SDL/index.php)

Phương pháp lập trình bàn cờ vua: [Chess Programming Wiki](https://www.chessprogramming.org)
