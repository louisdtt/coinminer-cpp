#include <iostream>
#include <openssl/sha.h>
#include <string.h>
#include <random>
#include <cstdlib>  
#include <stdio.h>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <sys/time.h>
#include <vector>

using namespace std;

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::system_clock;

class Miner{
    public:
        Miner() = default;
        virtual string asciiConvert(std::vector<int> charArr) = 0;
        virtual string genNonce() = 0;
        virtual string genCoin(string TriOwn) = 0;
        virtual inline void benchmark() = 0;
        virtual void mining(string triMiner,int cNumber) = 0;
};

class CoinMiner : public Miner {
    public:

        char Proto[6] = "CC1.0";
        char Reserved[7] ="0f0f0f";
        string coinBase;

        template <typename X>
        X CoinHash(X shaIn)
        {
            unsigned char hash[20];
            stringstream result;
            
            SHA_CTX sha;
            SHA1_Init(&sha);
            SHA1_Update(&sha, shaIn.c_str(), 60);
            SHA1_Final(hash, &sha);

            for (int i = 0; i < 20; i++)
            {
                result << hex << setw(2) << setfill('0') << (int)hash[i];
            }
            return result.str();
        }
      
        string asciiConvert(std::vector<int> charArr) override{
    
            string asciiRes;
            for (long unsigned int i = 0; i < charArr.size(); i++)
            {
                char character = charArr[i];
                asciiRes.push_back(character);
                i++;
            }
            return asciiRes;
        }

        string genNonce() override{
            std::vector<int> rdArr;
            int rdRes = 0;
            std::random_device rdNum;
            std::mt19937 gen(rdNum());
            std::uniform_int_distribution<> distr(32, 126);

            while (rdArr.size() != 64){
                rdRes = distr(gen);
                rdArr.push_back(rdRes);
            }
            return asciiConvert(rdArr);
        }

        string genCoin(string TriOwn) override{
            coinBase = genNonce()+"-"+TriOwn+"-"+Proto+"-"+to_string(time(0))+"-"+Reserved;
            return coinBase;  
        }

        inline void benchmark() override{
            string aCoin;
            string hCoin;
            int cCount = 0;
            auto timeStart = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
            long int durationTime;

            while(cCount < 1){
                aCoin = genCoin("LDU");
                hCoin = CoinHash<string>(aCoin);
                if (hCoin.at(0) == 'c' && hCoin.at(1) == 'c' && hCoin.at(2) == 'c' && hCoin.at(3) == 'c' && hCoin.at(4) == 'c')
                {
                    cCount++;
                }
            }
            auto timeEnd = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
            durationTime = (timeEnd - timeStart)/1000; // 1 coins * 1000ms

            cout << ("--------------------   Benchmark times   --------------------") <<endl;
            cout << ("SUBCOIN (6c)        " + to_string(durationTime*16) + " sec                  " + to_string(durationTime*16/3600)) + " h"<<endl;
            cout << ("COIN (7c)           " + to_string(durationTime*pow(16,2)) + " sec          " + to_string(durationTime*pow(16,2)/3600)) + " h"<<endl;
            cout << ("HEXCOIN (8c)        " + to_string(durationTime*pow(16,3)) + " sec         " + to_string(durationTime*pow(16,3)/3600)) + " h"<<endl;
            cout << ("ARKENSTONE (9c)     " + to_string(durationTime*pow(16,4)) + " sec       " + to_string(durationTime*pow(16,4)/3600)) + " h"<<endl;
            cout << ("BLACKSTAR (10c)     " + to_string(durationTime*pow(16,5)) + " sec      " + to_string(durationTime*pow(16,5)/3600)) + " h"<<endl;
            cout << ("GRANDCROSS (11c)    " + to_string(durationTime*pow(16,6)) + " sec     " + to_string(durationTime*pow(16,6)/3600)) + " h"<<endl;
            cout << ("-------------------------------------------------------------") <<endl;
        } 

        void mining(string triMiner,int cNumber) override{

            string aCoin;
            string hCoin;
            string strTemp;

            for (int i = 0; i < cNumber; i++)
            {
                strTemp.push_back('c');
            }

            while(true){

                aCoin = genCoin(triMiner);
                hCoin = CoinHash<string>(aCoin);
                
                if (hCoin.rfind(strTemp) == 0)
                {   
                    cout << aCoin << endl;
                }
            }
        }       

};

int main(int argc, char* argv[]) {

    auto* CoinCoinMiner = new CoinMiner();

    if (argc > 1)
    {
        try
        {
            if ((string)argv[1] == "-z"){
                    cout <<"Benchmark mode ..."<<endl;
                    CoinCoinMiner -> benchmark();
            }
            
            else if((string)argv[1] == "-t" && (string)argv[3] == "-m"){
                    CoinCoinMiner -> mining(argv[2], atoi(argv[4]));
            }

            else {
                throw invalid_argument("Arguments are invalid");
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    else{
        cout <<"You need to specify arguments (-z for benchmark or -t TRI -m value (value = min value of a coin to display))"<<endl;
    }

return 0;
}