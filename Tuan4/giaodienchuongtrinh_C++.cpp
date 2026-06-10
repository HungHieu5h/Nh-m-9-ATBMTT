#include <windows.h>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>

using namespace std;


#define ID_EDIT_P          101
#define ID_EDIT_G          102
#define ID_EDIT_X          103
#define ID_EDIT_Y          104
#define ID_BTN_AUTO_KEY    105
#define ID_BTN_APPLY_KEY   106

#define ID_EDIT_CONTENT    201
#define ID_BTN_LOAD_TEXT   202
#define ID_BTN_SIGN_SAVE   203
#define ID_STATIC_R        204
#define ID_STATIC_S        205

#define ID_BTN_VERIFY_LOAD 301
#define ID_BTN_VERIFY      302
#define ID_STATIC_STATUS   303


#define ID_BTN_RESET       400 


wstring ChuyenChieuRong(const string& str) {
    if (str.empty()) return L"";
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

string ChuyenChieuHep(const wstring& wstr) {
    if (wstr.empty()) return "";
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}


long long gcd(long long a, long long b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

long long modPow(long long base, long long exp, long long mod) {
    long long res = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % mod;
        exp = exp >> 1;
        base = (base * base) % mod;
    }
    return res;
}

long long modInverse(long long a, long long m) {
    long long m0 = m, t, q;
    long long x0 = 0, x1 = 1;
    if (m == 1) return 0;
    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m, a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0) x1 += m0;
    return x1;
}

long long generateK(long long p) {
    long long k;
    do {
        k = rand() % (p - 2) + 1; 
    } while (gcd(k, p - 1) != 1);
    return k;
}


void signElGamal(long long p, long long g, long long x, long long m, long long &r, long long &s) {
    long long k = generateK(p);
    r = modPow(g, k, p);
    long long k_inv = modInverse(k, p - 1);
    s = (k_inv * (m - x * r)) % (p - 1);
    if (s < 0) {
        s += (p - 1);
    }
}


bool verifyElGamal(long long p, long long g, long long y, long long m, long long r, long long s) {
    if (r <= 0 || r >= p || s <= 0 || s >= p - 1) {
        return false;
    }
    long long v1 = (modPow(y, r, p) * modPow(r, s, p)) % p;
    long long v2 = modPow(g, m, p);
    return v1 == v2;
}


bool laSoNguyenTo(long long n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (long long i = 5; i * i <= n; i = i + 6)
        if (n % i == 0 || n % (i + 2) == 0) return false;
    return true;
}

long long sinhSoNguyenToNgauNhien(long long minVal, long long maxVal) {
    long long p;
    do {
        p = minVal + rand() % (maxVal - minVal + 1);
    } while (!laSoNguyenTo(p));
    return p;
}


long long bamVanBan(wstring vanBan, long long soP) {
    long long m = 0;
    string utf8Str = ChuyenChieuHep(vanBan); 
    for (char c : utf8Str) {
        m = (m * 256 + (unsigned char)c) % soP; 
    }
    return m;
}


long long g_p = 0, g_g = 0, g_x = 0, g_y = 0;
bool g_daCoKhoa = false;

void luuFileText(string tenFile, wstring noiDung) {
    ofstream f(tenFile, ios::binary);
    if (f.is_open()) {
        string utf8Str = ChuyenChieuHep(noiDung);
        f << utf8Str;
        f.close();
    }
}

wstring docFileText(string tenFile) {
    ifstream f(tenFile, ios::binary);
    if (!f.is_open()) return L"";
    stringstream ss;
    ss << f.rdbuf();
    f.close();
    return ChuyenChieuRong(ss.str());
}


LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_CREATE: {
    
            HFONT hFont = CreateFontW(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");

         
            HWND g1 = CreateWindowW(L"BUTTON", L"VÙNG 1: QUẢN LÝ & SINH KHÓA", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 15, 10, 600, 170, hwnd, NULL, NULL, NULL);
            SendMessageW(g1, WM_SETFONT, (WPARAM)hFont, TRUE);

            CreateWindowW(L"STATIC", L"Số nguyên tố p:", WS_VISIBLE | WS_CHILD, 30, 40, 100, 20, hwnd, NULL, NULL, NULL);
            HWND eP = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 140, 40, 100, 20, hwnd, (HMENU)ID_EDIT_P, NULL, NULL);
            
            CreateWindowW(L"STATIC", L"Phần tử sinh g:", WS_VISIBLE | WS_CHILD, 30, 70, 100, 20, hwnd, NULL, NULL, NULL);
            HWND eG = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 140, 70, 100, 20, hwnd, (HMENU)ID_EDIT_G, NULL, NULL);

            CreateWindowW(L"STATIC", L"Khóa bí mật x:", WS_VISIBLE | WS_CHILD, 30, 100, 100, 20, hwnd, NULL, NULL, NULL);
            HWND eX = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 140, 100, 100, 20, hwnd, (HMENU)ID_EDIT_X, NULL, NULL);

            CreateWindowW(L"STATIC", L"Khóa công khai y:", WS_VISIBLE | WS_CHILD, 30, 130, 110, 20, hwnd, NULL, NULL, NULL);
            HWND eY = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY, 140, 130, 100, 20, hwnd, (HMENU)ID_EDIT_Y, NULL, NULL);

            HWND btnAuto = CreateWindowW(L"BUTTON", L"Sinh khóa tự động", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 270, 40, 160, 30, hwnd, (HMENU)ID_BTN_AUTO_KEY, NULL, NULL);
            HWND btnApply = CreateWindowW(L"BUTTON", L"Áp dụng & Tính Y", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 270, 85, 160, 30, hwnd, (HMENU)ID_BTN_APPLY_KEY, NULL, NULL);

          
            HWND g2 = CreateWindowW(L"BUTTON", L"VÙNG 2: KÝ VĂN BẢN (XỬ LÝ FILE)", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 15, 195, 600, 240, hwnd, NULL, NULL, NULL);
            SendMessageW(g2, WM_SETFONT, (WPARAM)hFont, TRUE);

            CreateWindowW(L"STATIC", L"Nội dung văn bản (Nhập tay hoặc tải từ file):", WS_VISIBLE | WS_CHILD, 30, 220, 350, 20, hwnd, NULL, NULL, NULL);
            HWND eContent = CreateWindowW(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | WS_VSCROLL | ES_AUTOVSCROLL, 30, 245, 560, 60, hwnd, (HMENU)ID_EDIT_CONTENT, NULL, NULL);

            HWND btnLoadTxt = CreateWindowW(L"BUTTON", L"Tải file văn bản", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 30, 320, 160, 30, hwnd, (HMENU)ID_BTN_LOAD_TEXT, NULL, NULL);
            HWND btnSign = CreateWindowW(L"BUTTON", L"Ký & Lưu file (vanban+chuky)", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 210, 320, 220, 30, hwnd, (HMENU)ID_BTN_SIGN_SAVE, NULL, NULL);

            CreateWindowW(L"STATIC", L"Chữ ký r:", WS_VISIBLE | WS_CHILD, 30, 370, 60, 20, hwnd, NULL, NULL, NULL);
            CreateWindowW(L"STATIC", L"Chưa có", WS_VISIBLE | WS_CHILD, 100, 370, 390, 20, hwnd, (HMENU)ID_STATIC_R, NULL, NULL);
            CreateWindowW(L"STATIC", L"Chữ ký s:", WS_VISIBLE | WS_CHILD, 30, 390, 60, 20, hwnd, NULL, NULL, NULL);
            CreateWindowW(L"STATIC", L"Chưa có", WS_VISIBLE | WS_CHILD, 100, 390, 390, 20, hwnd, (HMENU)ID_STATIC_S, NULL, NULL);

            
            HWND g3 = CreateWindowW(L"BUTTON", L"VÙNG 3: XÁC MINH CHỮ KÝ", WS_VISIBLE | WS_CHILD | BS_GROUPBOX, 15, 450, 600, 180, hwnd, NULL, NULL, NULL);
            SendMessageW(g3, WM_SETFONT, (WPARAM)hFont, TRUE);

            CreateWindowW(L"STATIC", L"Hệ thống sẽ tự động đọc dữ liệu từ cặp file 'vanban.txt' và 'chuky.txt' để xác thực tính toàn vẹn.", WS_VISIBLE | WS_CHILD, 30, 480, 560, 35, hwnd, NULL, NULL, NULL);
            HWND btnVerifyLoad = CreateWindowW(L"BUTTON", L"Đọc dữ liệu từ 2 File", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 30, 525, 180, 30, hwnd, (HMENU)ID_BTN_VERIFY_LOAD, NULL, NULL);
            HWND btnVerify = CreateWindowW(L"BUTTON", L"Xác minh toàn vẹn", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 230, 525, 180, 30, hwnd, (HMENU)ID_BTN_VERIFY, NULL, NULL);
            
            HWND sStatus = CreateWindowW(L"STATIC", L"Trạng thái kiểm tra: Chưa thực hiện", WS_VISIBLE | WS_CHILD, 30, 575, 560, 40, hwnd, (HMENU)ID_STATIC_STATUS, NULL, NULL);

            
            CreateWindowW(L"BUTTON", L"LÀM MỚI TOÀN BỘ GIAO DIỆN (RESET)", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 15, 645, 600, 35, hwnd, (HMENU)ID_BTN_RESET, NULL, NULL);

            
            EnumChildWindows(hwnd, [](HWND child, LPARAM font) -> BOOL {
                SendMessageW(child, WM_SETFONT, font, TRUE);
                return TRUE;
            }, (LPARAM)hFont);

            break;
        }

        case WM_COMMAND: {
            int wmId = LOWORD(wParam);
            
            
            if (wmId == ID_BTN_AUTO_KEY) {
                g_p = sinhSoNguyenToNgauNhien(1000, 9999);
                g_g = 2 + rand() % (g_p - 3);
                g_x = 2 + rand() % (g_p - 3);
                g_y = modPow(g_g, g_x, g_p); 
                g_daCoKhoa = true;

                SetDlgItemTextW(hwnd, ID_EDIT_P, to_wstring(g_p).c_str());
                SetDlgItemTextW(hwnd, ID_EDIT_G, to_wstring(g_g).c_str());
                SetDlgItemTextW(hwnd, ID_EDIT_X, to_wstring(g_x).c_str());
                SetDlgItemTextW(hwnd, ID_EDIT_Y, to_wstring(g_y).c_str());
                MessageBoxW(hwnd, L"Sinh khóa tự động thành công!", L"Thông báo", MB_OK | MB_ICONINFORMATION);
            }
            else if (wmId == ID_BTN_APPLY_KEY) {
                wchar_t bufP[32], bufG[32], bufX[32];
                GetDlgItemTextW(hwnd, ID_EDIT_P, bufP, 32);
                GetDlgItemTextW(hwnd, ID_EDIT_G, bufG, 32);
                GetDlgItemTextW(hwnd, ID_EDIT_X, bufX, 32);

                long long p = wcstoll(bufP, NULL, 10);
                long long g = wcstoll(bufG, NULL, 10);
                long long x = wcstoll(bufX, NULL, 10);

                if (!laSoNguyenTo(p)) {
                    MessageBoxW(hwnd, L"LỖI: Giá trị 'p' nhập vào không phải là số Nguyên Tố!", L"Lỗi nhập liệu", MB_OK | MB_ICONERROR);
                    g_daCoKhoa = false;
                    SetDlgItemTextW(hwnd, ID_EDIT_Y, L"Lỗi p");
                    return 0;
                }
                if (x <= 1 || x >= p - 1) {
                    MessageBoxW(hwnd, L"LỖI: Khóa bí mật x phải thỏa mãn điều kiện (1 < x < p-1)!", L"Lỗi logic", MB_OK | MB_ICONWARNING);
                    g_daCoKhoa = false;
                    return 0;
                }

                g_p = p; g_g = g; g_x = x;
                g_y = modPow(g_g, g_x, g_p); 
                g_daCoKhoa = true;
                SetDlgItemTextW(hwnd, ID_EDIT_Y, to_wstring(g_y).c_str());
                MessageBoxW(hwnd, L"Áp dụng bộ khóa thủ công thành công!", L"Thông báo", MB_OK | MB_ICONINFORMATION);
            }

            
            else if (wmId == ID_BTN_LOAD_TEXT) {
                wstring noiDung = docFileText("vanban.txt");
                if (noiDung.empty()) {
                    MessageBoxW(hwnd, L"Không tìm thấy file 'vanban.txt' hoặc file rỗng!", L"Lỗi đọc file", MB_OK | MB_ICONWARNING);
                } else {
                    SetDlgItemTextW(hwnd, ID_EDIT_CONTENT, noiDung.c_str());
                    MessageBoxW(hwnd, L"Tải nội dung file 'vanban.txt' thành công!", L"Thông báo", MB_OK | MB_ICONINFORMATION);
                }
            }
            else if (wmId == ID_BTN_SIGN_SAVE) {
                if (!g_daCoKhoa) {
                    MessageBoxW(hwnd, L"Vui lòng hoàn thành tạo hoặc cấu hình khóa hợp lệ ở Vùng 1 trước!", L"Cảnh báo", MB_OK | MB_ICONWARNING);
                    return 0;
                }

                int len = GetWindowTextLengthW(GetDlgItem(hwnd, ID_EDIT_CONTENT));
                wchar_t* bufContent = new wchar_t[len + 1];
                GetDlgItemTextW(hwnd, ID_EDIT_CONTENT, bufContent, len + 1);
                wstring vanBan(bufContent);
                delete[] bufContent;

                if (vanBan.empty()) {
                    MessageBoxW(hwnd, L"Nội dung văn bản trống, không thể ký!", L"Thông báo", MB_OK | MB_ICONWARNING);
                    return 0;
                }

                luuFileText("vanban.txt", vanBan);

                long long thongDiep_m = bamVanBan(vanBan, g_p);
                long long chuKy_r = 0, chuKy_s = 0;
                
                signElGamal(g_p, g_g, g_x, thongDiep_m, chuKy_r, chuKy_s);

                SetDlgItemTextW(hwnd, ID_STATIC_R, to_wstring(chuKy_r).c_str());
                SetDlgItemTextW(hwnd, ID_STATIC_S, to_wstring(chuKy_s).c_str());

                string chuKyStr = to_string(chuKy_r) + "\n" + to_string(chuKy_s);
                ofstream f("chuky.txt");
                if (f.is_open()) { f << chuKyStr; f.close(); }

                wstring msg = L"Đã thực hiện:\n1. Lưu văn bản vào 'vanban.txt'\n2. Gọi thuật toán signElGamal() tạo chữ ký thành công\n3. Lưu chữ ký vào 'chuky.txt'";
                MessageBoxW(hwnd, msg.c_str(), L"Kết quả ký file", MB_OK | MB_ICONINFORMATION);
            }

            
            else if (wmId == ID_BTN_VERIFY_LOAD) {
                wstring vanBan = docFileText("vanban.txt");
                ifstream fSign("chuky.txt");
                long long r = -1, s = -1;
                if (fSign.is_open()) { fSign >> r >> s; fSign.close(); }

                if (vanBan.empty() || r == -1 || s == -1) {
                    MessageBoxW(hwnd, L"Thiếu dữ liệu! Đảm bảo đã có file 'vanban.txt' và 'chuky.txt' trong thư mục chương trình.", L"Lỗi tải file", MB_OK | MB_ICONERROR);
                } else {
                    wstring info = L"Đã tải dữ liệu thành công:\n- Độ dài văn bản: " + to_wstring(vanBan.length()) + L" ký tự.\n- Cặp chữ ký đọc được (r, s): (" + to_wstring(r) + L", " + to_wstring(s) + L")";
                    MessageBoxW(hwnd, info.c_str(), L"Tải dữ liệu kiểm tra", MB_OK | MB_ICONINFORMATION);
                }
            }
            else if (wmId == ID_BTN_VERIFY) {
                if (!g_daCoKhoa) {
                    MessageBoxW(hwnd, L"Không tìm thấy thông tin khóa công khai để đối sánh. Vui lòng kiểm tra lại Vùng 1!", L"Lỗi xác minh", MB_OK | MB_ICONWARNING);
                    return 0;
                }

                wstring vanBan = docFileText("vanban.txt");
                ifstream fSign("chuky.txt");
                long long chuKy_r = -1, chuKy_s = -1;
                if (fSign.is_open()) { fSign >> chuKy_r >> chuKy_s; fSign.close(); }

                if (vanBan.empty() && (chuKy_r == -1 || chuKy_s == -1)) {
                    SetDlgItemTextW(hwnd, ID_STATIC_STATUS, L"Trạng thái kiểm tra: LỖI (Mất cả file văn bản và file chữ ký!)");
                    MessageBoxW(hwnd, L"LỖI HỆ THỐNG: Không tìm thấy cả file văn bản lẫn file chữ ký!", L"Thông báo lỗi", MB_OK | MB_ICONERROR);
                    return 0;
                }

                long long thongDiep_m = bamVanBan(vanBan, g_p);

                bool isValid = verifyElGamal(g_p, g_g, g_y, thongDiep_m, chuKy_r, chuKy_s);

                if (isValid) {
                    SetDlgItemTextW(hwnd, ID_STATIC_STATUS, L"Trạng thái kiểm tra: KHỚP CHUẨN - VĂN BẢN TOÀN VẸN 100%");
                    MessageBoxW(hwnd, L"XÁC MINH THÀNH CÔNG:\nChữ ký HOÀN TOÀN HỢP LỆ! Văn bản đảm bảo tính toàn vẹn và không bị sửa đổi.", L"Kết quả", MB_OK | MB_ICONINFORMATION);
                } else {
                    SetDlgItemTextW(hwnd, ID_STATIC_STATUS, L"Trạng thái kiểm tra: THẤT BẠI - DỮ LIỆU ĐÃ BỊ SỬA ĐỔI HOẶC KHÔNG KHỚP");
                    
                    wstring thongBaoLoi = L"CẢNH BÁO: CHỮ KÝ KHÔNG HỢP LỆ!\n\n"
                                         L"Hệ thống phát hiện bất thường, lỗi có thể rơi vào các tình huống sau:\n"
                                         L"1. Tình huống 1: Nội dung file 'vanban.txt' đã bị chỉnh sửa trái phép sau khi ký.\n"
                                         L"2. Tình huống 2: Các giá trị số trong file chữ ký 'chuky.txt' đã bị can thiệp thay đổi giả mạo.\n"
                                         L"3. Tình huống 3: Cả văn bản và file chữ ký đều không đồng bộ (bị sửa đổi đồng thời hoặc sai bộ khóa đối sánh).";
                    MessageBoxW(hwnd, thongBaoLoi.c_str(), L"Cảnh báo an toàn", MB_OK | MB_ICONERROR);
                }
            }

            
            else if (wmId == ID_BTN_RESET) {
                
                g_p = 0; g_g = 0; g_x = 0; g_y = 0;
                g_daCoKhoa = false;

                
                SetDlgItemTextW(hwnd, ID_EDIT_P, L"");
                SetDlgItemTextW(hwnd, ID_EDIT_G, L"");
                SetDlgItemTextW(hwnd, ID_EDIT_X, L"");
                SetDlgItemTextW(hwnd, ID_EDIT_Y, L"");
                SetDlgItemTextW(hwnd, ID_EDIT_CONTENT, L"");

                
                SetDlgItemTextW(hwnd, ID_STATIC_R, L"Chưa có");
                SetDlgItemTextW(hwnd, ID_STATIC_S, L"Chưa có");
                SetDlgItemTextW(hwnd, ID_STATIC_STATUS, L"Trạng thái kiểm tra: Chưa thực hiện");

                MessageBoxW(hwnd, L"Đã xóa sạch bộ nhớ tạm và đặt lại toàn bộ giao diện!", L"Reset Thành Công", MB_OK | MB_ICONINFORMATION);
            }
            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProcW(hwnd, message, wParam, lParam);
    }
    return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow) {
    srand(time(0));
    HWND hwnd;
    MSG messages;
    WNDCLASSEXW wincl;

    wincl.hInstance = hInstance;
    wincl.lpszClassName = L"ElGamalGUIPanel";
    wincl.lpfnWndProc = WindowProcedure;
    wincl.style = CS_DBLCLKS;
    wincl.cbSize = sizeof(WNDCLASSEXW);

    wincl.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;
    wincl.cbClsExtra = 0;
    wincl.cbWndExtra = 0;
    wincl.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);

    if (!RegisterClassExW(&wincl)) return 0;


    hwnd = CreateWindowExW(
        0, L"ElGamalGUIPanel", L"PHẦN MỀM CHỮ KÝ ĐIỆN TỬ ELGAMAL - ĐỒ ÁN MẬT MÃ HỌC",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 645, 730,
        HWND_DESKTOP, NULL, hInstance, NULL
    );

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessageW(&messages, NULL, 0, 0)) {
        TranslateMessage(&messages);
        DispatchMessageW(&messages);
    }

    return messages.wParam;
}