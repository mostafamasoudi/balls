using namespace std;
class blocks
{
  public:
    const int Bwidth = 80;
    const int Bheight = 50;
    int xrect, yrect, value;
    char number[4] = "";
    char convert(int value, char number[4])
    {
        for(int s=0;s<3;s++)
        number[s] =' ';
        int n = value;
        int i = 0;
        while (n > 0)
        {
            i++;
            n /= 10;
        }
        n = value;
        for (int j = i - 1; j >= 0; j--)
        {
            number[j] = '0' + n % 10;
            n /= 10;
        }
    }
    void touch()
    {
        value--;
    }
} rect[7][6];