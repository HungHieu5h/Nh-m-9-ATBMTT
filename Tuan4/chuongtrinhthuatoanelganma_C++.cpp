#include <windows.h>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;


long long uocChungLonNhat(long long soA, long long soB) {
    if (soB == 0) return soA;
    return uocChungLonNhat(soB, soA % soB);
}

long long luyThuaModulo(long long coSo, long long soMu, long long modulo) {
    if (modulo == 0) return 0;
    long long ketQua = 1;
    coSo = coSo % modulo;
    while (soMu > 0) {
        if (soMu % 2 == 1) ketQua = (ketQua * coSo) % modulo;
        soMu = soMu >> 1;
        coSo = (coSo * coSo) % modulo;
    }
    return ketQua;
}

long long timNghichDaoModulo(long long soA, long long moduloM) {
    long long m_goc = moduloM, tam, thuong;
    long long x0 = 0, x1 = 1;
    if (moduloM == 1) return 0;
    while (soA > 1) {
        thuong = soA / moduloM;
        tam = moduloM;
        moduloM = soA % moduloM;
        soA = tam;
        tam = x0;
        x0 = x1 - thuong * x0;
        x1 = tam;
    }
    if (x1 < 0) x1 += m_goc;
    return x1;
}

long long sinhKNgauNhien(long long soP) {
    if (soP <= 2) return 1;
    long long soK;
    do {
        soK = rand() % (soP - 2) + 1;
    } while (uocChungLonNhat(soK, soP - 1) != 1);
    return soK;
}


#define ID_NUT_KY 101
#define ID_NUT_XACMINH 102

HWND o_soP, o_soG, o_khoaX, o_thongDiepM, o_khoaY, o_chuKyR, o_chuKyS;
HWND o_mXacMinh, o_rXacMinh, o_sXacMinh;

long long laySoTuO(HWND oNhap) {
    if (!oNhap) return 0;
    wchar_t text[32] = {0};
    GetWindowTextW(oNhap, text, 32);
    return wcstoll(text, NULL, 10);
}

void datSoVaoO(HWND oHienThi, long long so) {
    if (!oHienThi) return;
    wstring s = to_wstring(so);
    SetWindowTextW(oHienThi, s.c_str());
}

LRESULT CALLBACK HamXuLyCuaSo(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE: {
        
            CreateWindowW(L"Static", L"SỐ NGUYÊN TỐ P:", WS_VISIBLE | WS_CHILD, 20, 20, 120, 20, hwnd, NULL, NULL, NULL);
            o_soP = CreateWindowW(L"Edit", L"11", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 150, 20, 80, 20, hwnd, NULL, NULL, NULL);

            CreateWindowW(L"Static", L"PHẦN TỬ SINH G:", WS_VISIBLE | WS_CHILD, 20, 50, 120, 20, hwnd, NULL, NULL, NULL);
            o_soG = CreateWindowW(L"Edit", L"2", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 150, 50, 80, 20, hwnd, NULL, NULL, NULL);

            CreateWindowW(L"Static", L"KHÓA BÍ MẬT X:", WS_VISIBLE | WS_CHILD, 20, 80, 120, 20, hwnd, NULL, NULL, NULL);
            o_khoaX = CreateWindowW(L"Edit", L"8", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 150, 80, 80, 20, hwnd, NULL, NULL, NULL);

          
            CreateWindowW(L"Static", L"--- CHỨC NĂNG KÝ ---", WS_VISIBLE | WS_CHILD, 20, 120, 200, 20, hwnd, NULL, NULL, NULL);
            
            CreateWindowW(L"Static", L"Nhập thông điệp M:", WS_VISIBLE | WS_CHILD, 20, 150, 130, 20, hwnd, NULL, NULL, NULL);
            o_thongDiepM = CreateWindowW(L"Edit", L"5", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 150, 150, 80, 20, hwnd, NULL, NULL, NULL);

            CreateWindowW(L"Button", L"TẠO CHỮ KÝ", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 20, 180, 210, 30, hwnd, (HMENU)ID_NUT_KY, NULL, NULL);

            CreateWindowW(L"Static", L"Khóa công khai Y:", WS_VISIBLE | WS_CHILD, 20, 220, 120, 20, hwnd, NULL, NULL, NULL);
            o_khoaY = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY, 150, 220, 80, 20, hwnd, NULL, NULL, NULL);

            CreateWindowW(L"Static", L"Chữ ký r:", WS_VISIBLE | WS_CHILD, 20, 250, 120, 20, hwnd, NULL, NULL, NULL);
            o_chuKyR = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY, 150, 250, 80, 20, hwnd, NULL, NULL, NULL);

            CreateWindowW(L"Static", L"Chữ ký s:", WS_VISIBLE | WS_CHILD, 20, 280, 120, 20, hwnd, NULL, NULL, NULL);
            o_chuKyS = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY, 150, 280, 80, 20, hwnd, NULL, NULL, NULL);

         
            CreateWindowW(L"Static", L"--- CHỨC NĂNG XÁC MINH ---", WS_VISIBLE | WS_CHILD, 280, 120, 200, 20, hwnd, NULL, NULL, NULL);

            CreateWindowW(L"Static", L"M cần xác minh:", WS_VISIBLE | WS_CHILD, 280, 150, 120, 20, hwnd, NULL, NULL, NULL);
            o_mXacMinh = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 400, 150, 80, 20, hwnd, NULL, NULL, NULL);

            CreateWindowW(L"Static", L"Nhập chữ ký r:", WS_VISIBLE | WS_CHILD, 280, 180, 120, 20, hwnd, NULL, NULL, NULL);
            o_rXacMinh = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 400, 180, 80, 20, hwnd, NULL, NULL, NULL);

            CreateWindowW(L"Static", L"Nhập chữ ký s:", WS_VISIBLE | WS_CHILD, 280, 210, 120, 20, hwnd, NULL, NULL, NULL);
            o_sXacMinh = CreateWindowW(L"Edit", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER, 400, 210, 80, 20, hwnd, NULL, NULL, NULL);

            CreateWindowW(L"Button", L"XÁC MINH CHỮ KÝ", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 280, 250, 200, 30, hwnd, (HMENU)ID_NUT_XACMINH, NULL, NULL);
            break;
        }
        case WM_COMMAND: {
            if (LOWORD(wp) == ID_NUT_KY) {
                long long p = laySoTuO(o_soP);
                long long g = laySoTuO(o_soG);
                long long x = laySoTuO(o_khoaX);
                long long m = laySoTuO(o_thongDiepM);

                if (p <= 0) {
                    MessageBoxW(hwnd, L"Lỗi: Số nguyên tố p phải lớn hơn 0!", L"Thông báo lỗi", MB_OK | MB_ICONERROR);
                    break;
                }
                if (m >= p) {
                    MessageBoxW(hwnd, L"Lỗi: Thông điệp M phải nhỏ hơn p!", L"Thông báo lỗi", MB_OK | MB_ICONERROR);
                    break;
                }
                
                long long y = luyThuaModulo(g, x, p);
                long long r, s;
                long long k = sinhKNgauNhien(p);
                r = luyThuaModulo(g, k, p);
                long long k_nghichDao = timNghichDaoModulo(k, p - 1);
                s = (k_nghichDao * (m - x * r)) % (p - 1);
                if (s < 0) s += (p - 1);

                datSoVaoO(o_khoaY, y);
                datSoVaoO(o_chuKyR, r);
                datSoVaoO(o_chuKyS, s);
                
                datSoVaoO(o_mXacMinh, m);
                datSoVaoO(o_rXacMinh, r);
                datSoVaoO(o_sXacMinh, s);
            }
            
            if (LOWORD(wp) == ID_NUT_XACMINH) {
                long long p = laySoTuO(o_soP);
                long long g = laySoTuO(o_soG);
                long long x = laySoTuO(o_khoaX);
                
                long long m_xm = laySoTuO(o_mXacMinh);
                long long r_xm = laySoTuO(o_rXacMinh);
                long long s_xm = laySoTuO(o_sXacMinh);

                if (p <= 0 || r_xm <= 0 || r_xm >= p || s_xm <= 0 || s_xm >= p - 1) {
                    MessageBoxW(hwnd, L"Chữ ký KHÔNG HỢP LỆ! (Sai khoảng giá trị)", L"Kết quả", MB_OK | MB_ICONERROR);
                    break;
                }

                long long y = luyThuaModulo(g, x, p);
                long long v1 = (luyThuaModulo(y, r_xm, p) * luyThuaModulo(r_xm, s_xm, p)) % p;
                long long v2 = luyThuaModulo(g, m_xm, p);

                if (v1 == v2) {
                    wstring message = L"Chữ ký HỢP LỆ!\nThông điệp toàn vẹn.\n(v1 = v2 = " + to_wstring(v1) + L")";
                    MessageBoxW(hwnd, message.c_str(), L"Kết quả xác minh", MB_OK | MB_ICONINFORMATION);
                } else {
                    wstring message = L"Chữ ký KHÔNG HỢP LỆ!\nThông điệp đã bị sửa đổi!\n(v1 = " + to_wstring(v1) + L" , v2 = " + to_wstring(v2) + L")";
                    MessageBoxW(hwnd, message.c_str(), L"Kết quả xác minh", MB_OK | MB_ICONWARNING);
                }
            }
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProcW(hwnd, msg, wp, lp);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {
    srand(time(0));
    
    WNDCLASSW wc = {0};
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = hInst;
    wc.lpszClassName = L"GiaoDienElGamal";
    wc.lpfnWndProc = HamXuLyCuaSo;

    if (!RegisterClassW(&wc)) return -1;

    CreateWindowW(L"GiaoDienElGamal", L"CHƯƠNG TRÌNH CHỮ KÝ ĐIỆN TỬ ELGAMAL", 
                  WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE, 
                  100, 100, 520, 370, NULL, NULL, NULL, NULL);

    MSG msg = {0};
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return 0;
}