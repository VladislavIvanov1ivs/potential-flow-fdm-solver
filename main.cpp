
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <Eigen/Sparse>
using namespace Eigen;
using namespace std;
int PoiskN(int cols, int stroka, int stolbec)
{
    return stroka * cols + 1 + stolbec;
};
int PoiskJ(int Num, int Nx)
{
    return (Num - 1) % Nx;
};
int PoiskI(int Num, int Nx)
{
    return (Num - 1) / Nx;
};
void inFileAdd(string path, double x)
{
    ofstream fout;
    fout.open(path, ofstream::app);
    if (!fout.is_open())
    {
        cout << "\n К сожалению файл с именем namefile=" << path << " не был открыт!";
    }
    else
    {
        fout << x;
    };
    fout.close();
};
void inFileAdd(string path, int x)
{
    ofstream fout;
    fout.open(path, ofstream::app);
    if (!fout.is_open())
    {
        cout << "\n К сожалению файл с именем namefile=" << path << " не был открыт!";
    }
    else
    {
        fout << x;
        fout << endl;
    };
    fout.close();
};
double Delta(double l, int n)
{
    return l / n;
};
class Telo;
double Delta(double l, int n);
class Soedinenie;
class Mesh;
class Reshatel;
class Mesh
{
    friend Soedinenie;
    friend Reshatel;
    friend Telo;
private:
    int Nx, Ny;
    double* Xp;
    double* Yp;
    double lx, ly, deltax, deltay;
    bool pic;
public:
    void SetNxNy()
    {
        cout << "\nЖелаете ли вы работать с произвольным профилем?\t (Если да нажмите 1, если нет нажмите 0)\n";
        cin >> pic;
        if (pic == 0)
        {
            cout << "\nВВедите число разбиений вдоль оси Ox:: Nx=\n";
            cin >> Nx;
            cout << "\nВВедите число разбиений вдоль оси Oy:: Ny=\n";
            cin >> Ny;
        }
        else
        {
            int width, height, channels;
            unsigned char* img = stbi_load("telo.png", &width, &height, &channels, 3);
            if (!img)
            {
                cerr << "\n Ошибка открытия файла telo.png!\n";
            }
            else {
                Nx = width;
                Ny = height;

            };
            stbi_image_free(img);
        };

        deltax = Delta(lx, Nx);
        deltay = Delta(ly, Ny);
    };
    void SetGr()
    {
        cout << "\nВВедите длину области вдоль оси Ox:: Lx=\n";
        cin >> lx;
        cout << "\nВВедите длину области вдоль оси Oy:: Ly=\n";
        cin >> ly;

    };
    void GenArr()
    {
        Yp = new double[Ny + 1];
        Xp = new double[Nx + 1];

    };
    void GenerationMesh()
    {

        for (int i = 0; i < Nx + 1; i++)
        {
            Xp[i] = i * deltax;
        };
        for (int i = 0; i < Ny + 1; i++)
        {
            Yp[i] = i * deltay;
        };
    };
    void Save()
    {
        ofstream fout;
        fout.open("SaverKO.txt", ofstream::app);
        if (!fout.is_open())
        {
            cout << "\n К сожалению файл с именем namefile= SaverKO.txt не был открыт!";
        }
        else
        {
            fout << "===================Xp-Yp================\n";
            fout << "ВНИМАНИЕ ПЕРВАЯ(ВЕРХНЯЯ СТРОКА)- КООРДИНАТА X; СО ВТОРОЙ ПО N СТОРКУ КООРДИНАТЫ Y;\n";
            for (int i = 0; i < Nx + 1; i++)
            {
                fout << "\t" << Xp[i];
            };
            fout << endl;
            for (int i = 0; i < Ny + 1; i++)
            {
                for (int j = 0; j < Nx + 1; j++)
                {
                    fout << "\t" << Yp[i];
                };
                fout << endl;
            };
        };
        fout.close();

    };
    void DeleteMesh()
    {
        delete[] Xp;
        delete[] Yp;
    };
    ~Mesh()
    {
        cout << "\nВызвался деструктор\n";
    };
};
class Telo
{
    friend Soedinenie;
private:
    int Otvet;
    double r = 0, ax = 0, by = 0, R = 0;

public:
    void Tekst(Mesh& m)
    {
        if (m.pic == 0)
        {
            cout << "\n Какое тело вы хотели бы обтекать?\nНа выбор: \n1) Цилиндр;\n2)Квадрат;\n3)Крыловой профиль\n";
            cin >> Otvet;
            while ((Otvet < 1) || (Otvet > 2))
            {
                cout << "\nИзвините но Вы ввели номер не из списка, повторите еще раз!";
                cout << "\n Какое тело вы хотели бы обтекать?\nНа выбор: \n1) Цилиндр;\n2)Квадрат;\n";
                cin >> Otvet;
            };
        };

    };
    void Case(Mesh& m)
    {
        if (m.pic == 0)
        {
            switch (Otvet)
            {
            case (1):
                cout << "Вы выбрали цилидр, введите его радиус R=\n";
                cin >> r;
                break;
            case (2):
                cout << "Вы выбрали прямоугольник, введите его сторону a (вдоль оси Ox)=\n";
                cin >> ax;
                cout << "Вы выбрали прямоугольник, введите его сторону b (вдоль оси Oy)=\n";
                cin >> by;
                break;
            };
        };

    };
};
class Soedinenie {
    friend Reshatel;
private:
    double x0, y0;
    vector<int> body_nodes; // Узлы, принадлежащие телу (включая границу)

public:
    void Centr(Mesh& mesh) {
        if (mesh.pic == false) {
            x0 = mesh.lx / 2;
            y0 = mesh.ly / 2;
        }
    };

    void Iskl(Mesh& mesh, Telo& telo) {
        if (mesh.pic == false) {
            string p = "SaveIsklKO.txt";
            ofstream fout(p);
            fout.close(); // Очищаем файл

            body_nodes.clear();

            switch (telo.Otvet) {
            case (1): // Цилиндр
                for (int i = 1; i < mesh.Nx; i++) {
                    for (int j = 1; j < mesh.Ny; j++) {
                        double dist = sqrt(pow(mesh.Xp[i] - x0, 2) + pow(mesh.Yp[j] - y0, 2));
                        if (dist <= telo.r) {
                            int node = PoiskN(mesh.Nx + 1, j, i);
                            body_nodes.push_back(node);
                        }
                    }
                }
                break;
            case (2): // Квадрат
                for (int i = 1; i < mesh.Nx; i++) {
                    for (int j = 1; j < mesh.Ny; j++) {
                        if ((mesh.Xp[i] >= x0 - telo.ax / 2) && (mesh.Xp[i] <= x0 + telo.ax / 2) &&
                            (mesh.Yp[j] >= y0 - telo.by / 2) && (mesh.Yp[j] <= y0 + telo.by / 2)) {
                            int node = PoiskN(mesh.Nx + 1, j, i);
                            body_nodes.push_back(node);
                        }
                    }
                }
                break;
            }

            // Сохраняем все узлы тела
            for (int node : body_nodes) {
                inFileAdd(p, node);
            }
        }
        else {
            // Обработка изображения (тело — всё, что не белое)
            int width, height, channels;
            unsigned char* img = stbi_load("telo.png", &width, &height, &channels, 3);
            if (!img) {
                cerr << "Ошибка загрузки изображения!" << endl;
                return;
            }

            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int idx = (y * width + x) * channels;
                    unsigned char r = img[idx];
                    unsigned char g = img[idx + 1];
                    unsigned char b = img[idx + 2];

                    // Если пиксель не белый (RGB=255,255,255) — это тело
                    if (!(r == 255 && g == 255 && b == 255)) {
                        // Переводим координаты изображения в сетку
                        int i = (x * mesh.Nx) / width;
                        int j = (y * mesh.Ny) / height;
                        int node = PoiskN(mesh.Nx + 1, j, i);
                        body_nodes.push_back(node);
                    }
                }
            }
            stbi_image_free(img);
        }

        // Сохранение узлов тела 
        ofstream fout("SaveIsklKO.txt");
        for (int node : body_nodes) fout << node << endl;
        fout.close();
    }
};
class Reshatel
{
    friend Mesh;

public:
    void SetUsl(Mesh& m)
    {
        Nmax = (m.Nx + 1) * (m.Ny + 1);
        Psi.resize(Nmax);
        Psi.setZero();
    };
    void SetGU(Mesh& m, Soedinenie& s) {
        cout << "\n Введите значение скорости в камере\n";
        cin >> U;

        SparseMatrix<double> A(Nmax, Nmax);
        VectorXd B(Nmax);
        double dx = m.deltax, dy = m.deltay;
        B.setZero();

        // Заполнение матрицы для внутренних узлов
        for (int i = 1; i < m.Nx; i++) {
            for (int j = 1; j < m.Ny; j++) {
                int node = PoiskN(m.Nx + 1, j, i) - 1;

                // Проверяем, не является ли узел частью тела
                bool is_body = false;
                for (int body_node : s.body_nodes) {
                    if (node == body_node - 1) {
                        is_body = true;
                        break;
                    }
                }

                if (!is_body) {
                    A.insert(node, node) = -2 * (1 / (dx * dx) + 1 / (dy * dy));
                    A.insert(node, PoiskN(m.Nx + 1, j + 1, i) - 1) = 1 / (dy * dy);
                    A.insert(node, PoiskN(m.Nx + 1, j - 1, i) - 1) = 1 / (dy * dy);
                    A.insert(node, PoiskN(m.Nx + 1, j, i + 1) - 1) = 1 / (dx * dx);
                    A.insert(node, PoiskN(m.Nx + 1, j, i - 1) - 1) = 1 / (dx * dx);
                }
            }
        }

        // Граничные условия на входе/выходе 
        for (int j = 0; j <= m.Ny; j++) {
            int inlet_node = PoiskN(m.Nx + 1, j, 0) - 1;
            A.coeffRef(inlet_node, inlet_node) = 1;
            B(inlet_node) = U * m.Yp[j];
        }
        for (int j = 0; j <= m.Ny; j++) {
            int outlet_node = PoiskN(m.Nx + 1, j, m.Nx) - 1;
            int prev_node1 = PoiskN(m.Nx + 1, j, m.Nx - 1) - 1;
            int prev_node2 = PoiskN(m.Nx + 1, j, m.Nx - 2) - 1;

            A.coeffRef(outlet_node, outlet_node) = 3;
            A.coeffRef(outlet_node, prev_node1) = -4;
            A.coeffRef(outlet_node, prev_node2) = 1;
            B(outlet_node) = 0;
        }

        // Граничные условия на верхней и нижней стенках
        for (int i = 1; i < m.Nx; i++) {
            int top_node = PoiskN(m.Nx + 1, m.Ny, i) - 1;
            int bottom_node = PoiskN(m.Nx + 1, 0, i) - 1;

            A.coeffRef(top_node, top_node) = 1;
            A.coeffRef(bottom_node, bottom_node) = 1;
            B(top_node) = U * m.ly;
            B(bottom_node) = 0;
        }

        // Условия для тела: ψ = const
        for (int node : s.body_nodes) {
            int node_idx = node - 1;
            A.coeffRef(node_idx, node_idx) = 1;
            B(node_idx) = U*m.ly/2;
        }


        A.makeCompressed();

        // Решение системы
        SparseLU<SparseMatrix<double>> solver;
        solver.compute(A);
        Psi = solver.solve(B);

        SaveResults(m, s);
        SaveStreamlines(m, s);
    }
    void SaveResults(Mesh& m, Soedinenie& s) {
        ofstream fx("Xcoordinate.txt");
        ofstream fy("Ycoordinate.txt");
        ofstream fpsi("Reshenie.txt");
        ofstream fvel("Velocity.txt");
        fvel << "X\tY\tU\tV\tVelocity\tIsBody\n";

        for (int j = 0; j <= m.Ny; j++) {
            for (int i = 0; i <= m.Nx; i++) {
                int node = PoiskN(m.Nx + 1, j, i) - 1;
                bool is_body = false;
                for (int body_node : s.body_nodes) {
                    if (node == body_node - 1) is_body = true;
                }

                // Запись координат и ψ
                fx << m.Xp[i] << "\n";
                fy << m.Yp[j] << "\n";
                fpsi << (is_body ? 0 : Psi(node)) << "\n";

                // Инициализация скоростей
                double u = 0, v = 0;

                if (!is_body) {
                    // Вычисление u = ∂ψ/∂y с особым учетом предпоследних узлов по Y
                    if (j == 0) {
                        u = (-3 * Psi(node) + 4 * Psi(PoiskN(m.Nx + 1, j + 1, i)) -
                            Psi(PoiskN(m.Nx + 1, j + 2, i))) / (2 * m.deltay);
                    }
                    else if (j == m.Ny) {
                        u = (3 * Psi(node) - 4 * Psi(PoiskN(m.Nx + 1, j - 1, i)) +
                            Psi(PoiskN(m.Nx + 1, j - 2, i))) / (2 * m.deltay);
                    }
                    else if (j == m.Ny - 1) {
                        // Специальная обработка предпоследнего узла по Y
                        u = (Psi(PoiskN(m.Nx + 1, j + 1, i)) -
                            Psi(PoiskN(m.Nx + 1, j - 1, i))) / (2 * m.deltay);
                    }
                    else {
                        u = (Psi(PoiskN(m.Nx + 1, j + 1, i)) -
                            Psi(PoiskN(m.Nx + 1, j - 1, i))) / (2 * m.deltay);
                    }

                    // Вычисление v = -∂ψ/∂x с особым учетом последних узлов по X
                    if (i == 0) {
                        v = -(-3 * Psi(node) + 4 * Psi(PoiskN(m.Nx + 1, j, i + 1)) -
                            Psi(PoiskN(m.Nx + 1, j, i + 2))) / (2 * m.deltax);
                    }
                    else if (i == m.Nx) {
                        // Для последнего узла по X используем одностороннюю разность
                        v = -(3 * Psi(node) - 4 * Psi(PoiskN(m.Nx + 1, j, i - 1)) +
                            Psi(PoiskN(m.Nx + 1, j, i - 2))) / (2 * m.deltax);
                    }
                    else if (i == m.Nx - 1) {
                        // Специальная обработка предпоследнего узла по X
                        v = -(Psi(PoiskN(m.Nx + 1, j, i + 1)) -
                            Psi(PoiskN(m.Nx + 1, j, i - 1))) / (2 * m.deltax);
                    }
                    else {
                        v = -(Psi(PoiskN(m.Nx + 1, j, i + 1)) -
                            Psi(PoiskN(m.Nx + 1, j, i - 1))) / (2 * m.deltax);
                    }
                }

                // Вычисление модуля скорости с проверкой на NaN
                double Vel = sqrt(u * u + v * v);
                if (isnan(Vel)) Vel = 0;
                if (Vel>=4*U) Vel /= 10;
                if ((i != m.Nx - 1) or (i != m.Nx))
                {
                    fvel << m.Xp[i] << "\t" << m.Yp[j] << "\t"
                        << (is_body ? 0 : u) << "\t" << (is_body ? 0 : v) << "\t"
                        << Vel << "\t" << (is_body ? 1 : 0) << "\n";
                };
            }
        }

        fx.close();
        fy.close();
        fpsi.close();
        fvel.close();
    }
    void SaveStreamlines(Mesh& m, Soedinenie& s) {
        ofstream fstream("Streamlines.txt");
        if (!fstream.is_open()) {
            cerr << "Ошибка открытия файла Streamlines.txt" << endl;
            return;
        }

        // Определяем шаг 
        int step_y = max(1, m.Ny / 50); 
        int step_x = max(1, m.Nx / 50); 

        fstream << "X\tY\tPsi\tIsBody\n";

        for (int j = 0; j <= m.Ny; j += step_y) {
            for (int i = 0; i <= m.Nx; i += step_x) {
                int node = PoiskN(m.Nx + 1, j, i) - 1;
                bool is_body = false;

                // Проверяем, принадлежит ли узел телу
                for (int body_node : s.body_nodes) {
                    if (node == body_node - 1) {
                        is_body = true;
                        break;
                    }
                }

                fstream << m.Xp[i] << "\t" << m.Yp[j] << "\t"
                    << (is_body ? 0 : Psi(node)) << "\t"
                    << (is_body ? 1 : 0) << "\n";
            }
        }

        fstream.close();
    }

private:
    double U;
    int Nmax;
    VectorXd Psi;
};

int main()
{
    setlocale(LC_ALL, "Rus");
    Mesh a;
    a.SetGr();
    a.SetNxNy();
    a.GenArr();
    a.GenerationMesh();
    a.Save();
    Telo b;
    b.Tekst(a);
    b.Case(a);
    Soedinenie c;
    c.Centr(a);
    c.Iskl(a, b);
    Reshatel d;
    d.SetUsl(a);
    d.SetGU(a, c);
    a.DeleteMesh();

};




