#define OPENSSL_NO_KRB5
#include <iostream>
#include <string>
#include <vector>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
using namespace std;

void dbg(const string &s) { cout << s << endl; }

typedef unsigned char uchar;

string to_hex(uchar ch)
{
    char buf[5];
    sprintf(buf, "%02X", ch);
    return string(buf);
}

uchar from_hex(const string &s)
{
    uchar ch;
    sscanf(s.c_str(), "%02X", &ch);
    return ch;
}

/**
@brief Класс шифрования данных
*/
class crypto_t
{
    string BIO_to_string(BIO *bio)
    {
        const int len = BIO_pending(bio);

        uchar *buf = new uchar[len + 1];
        memset(buf, 0, len + 1);
        BIO_read(bio, buf, len);

        string res;
        for (int i = 0; i < len; ++i)
            res += to_hex(buf[i]);

        delete[] buf;

        return res;
    }

    BIO * string_to_BIO(const string &s)
    {
        BIO *bio = NULL;
        if (s.size() & 1)
            return NULL;

        cout << s << endl;
        const int len = s.size() / 2;
        uchar *buf = new uchar[len + 1];
        memset(buf, 0, len + 1);
        for (int i = 0; i < len; ++i)
            buf[i] = from_hex(s.substr(2 * i, 2));
        cout << int(buf[0]) << endl;
        cout << int(buf[1]) << endl;
        cout << int(buf[2]) << endl;
        if (BIO_write(bio, buf, len) == 0)
            cout << "BBABAAAD!" << endl;
        
        delete[] buf;
        
        return bio;
    }

    string public_key;
    string private_key;

public:

    /**
    @brief Инициализация приватного и публичного ключей
    @param name - имена ключей
    */
    crypto_t()
    {
        OpenSSL_add_all_algorithms();

        // генерируем ключи
        RSA *rsa = RSA_generate_key(2048, RSA_F4, 0, 0);

        // сохраняем публичный ключ
        BIO *bio = BIO_new(BIO_s_mem());
        PEM_write_bio_RSAPublicKey(bio, rsa);
        public_key = BIO_to_string(bio);
        BIO_free_all(bio);

        // сохраняем приватный ключ

        bio = BIO_new(BIO_s_mem());
        PEM_write_bio_RSAPrivateKey(bio, rsa, 0, 0, 0, 0, 0);
        private_key = BIO_to_string(bio);
        BIO_free_all(bio);

        // очищаем память
        RSA_free(rsa);
    }

    ~crypto_t()
    {
        EVP_cleanup();
    }

    /**
    @brief Возвращает публичный свой ключ
    */
    string get_public_key()
    {
        return public_key;
    }

    /**
    @brief Расшифровывает сообщение msg своим приватным ключем
    @param msg - зашифрованное сообщение
    */
    string decrypt(const string &msg)
    {
        BIO *bio = string_to_BIO(private_key);
        RSA *priv_key = PEM_read_bio_RSAPrivateKey(bio, 0, 0, 0);

        string ans;
        int len = RSA_size(priv_key);
        for (int i = 0; i < msg.size(); i += len)
        {
            string s = msg.substr(i, len);
            unsigned char *from = new unsigned char[len + 1];
            memset(from, 0, len + 1);
            memcpy(from, s.c_str(), len);

            unsigned char *to = NULL;
            int olen = RSA_private_decrypt(len, from, to, priv_key, RSA_PKCS1_PADDING);
            if (olen < 0)
            {
                delete[] from;
                delete[] to;
                return string();
            }

            ans += string(reinterpret_cast<char *>(to));
            

            delete[] from;
            delete[] to;
        }

        BIO_free_all(bio);
        RSA_free(priv_key);

        return ans;
    }

    /**
    @brief Шифрует сообщение msg при помощи публичного ключа public_key
    @param msg - сообщение
    @param public_key - публичный ключ
    */
    string encrypt(const string &msg, const string &public_key)
    {
        cerr << "here1!" << endl;
        BIO *bio = string_to_BIO(public_key);

        if (bio == 0)
            cout << "bad!" << endl;

        cerr << "here2!" << endl;
        RSA *pub_key = PEM_read_bio_RSAPublicKey(bio, 0, 0, 0);
        cerr << "here3!" << endl;

        string ans;

        int len = RSA_size(pub_key) - 11;
        for (int i = 0; i < msg.size(); i += len - 1)
        {
            string s = msg.substr(i, len - 1);
            unsigned char *from = new unsigned char[len];
            memset(from, 0, len);
            memcpy(from, s.c_str(), s.size());

            unsigned char *to = NULL;
            int olen = RSA_public_encrypt(len, from, to, pub_key, RSA_PKCS1_PADDING);
            if (olen != RSA_size(pub_key))
            {
                delete[] from;
                delete[] to;
                return string();
            }
            
            ans += string(reinterpret_cast<char *>(to));
            

            delete[] from;
            delete[] to;
        }

        BIO_free_all(bio);
        RSA_free(pub_key);

        return ans;
    }
};

int main()
{
    crypto_t u;
    crypto_t v;

    string pu = u.get_public_key();
    string pv = v.get_public_key();
    
    string tu = "hello!";
    string cu = u.encrypt(tu, pv);
    dbg(cu);

    return 0;
}