#include <napi.h>
#include <uv.h>
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

#define CHECK_UV_RES(uv_res) \
{ \
    if (uv_res) \
    { \
        Napi::Error::New(env, uv_strerror(uv_res)).ThrowAsJavaScriptException(); \
        return env.Undefined(); \
    } \
}

Napi::Value getFreePort(const Napi::CallbackInfo& info)
{
    auto env = info.Env();
    uv_tcp_t* server = new uv_tcp_t();
    CHECK_UV_RES(uv_tcp_init(uv_default_loop(), server));
    struct sockaddr_in addr;
    CHECK_UV_RES(uv_ip4_addr("127.0.0.1", 0, &addr));
    CHECK_UV_RES(uv_tcp_bind(server, reinterpret_cast<const struct sockaddr*>(&addr), 0));
    struct sockaddr_storage addr_storage;
    int addr_len = sizeof(addr_storage);
    CHECK_UV_RES(uv_tcp_getsockname(server, reinterpret_cast<struct sockaddr*>(&addr_storage), &addr_len));
    int port;
    if (addr_storage.ss_family==AF_INET)
    {
        auto addr_in = reinterpret_cast<struct sockaddr_in*>(&addr_storage);
        port = ntohs(addr_in->sin_port);
    }
    else
    {
        auto addr_in6 = reinterpret_cast<struct sockaddr_in6*>(&addr_storage);
        port = ntohs(addr_in6->sin6_port);
    }
    uv_close(reinterpret_cast<uv_handle_t*>(server),
        [](uv_handle_t* handle){ delete handle; });
    return Napi::Number::New(env, port);
}

Napi::Object init(Napi::Env env, Napi::Object exports)
{
    exports["getFreePort"] = Napi::Function::New(env, getFreePort);
    return exports;
}

NODE_API_MODULE(get_free_port, init)
