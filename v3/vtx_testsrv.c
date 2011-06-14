//
//  VTX test example, server
//
//  This file is part of VTX, the 0MQ virtual transport interface:
//  http://vtx.zeromq.org.

#include "vtx.c"
#include "vtx_udp.c"

int main (void)
{
    //  Initialize 0MQ context and virtual transport interface
    zctx_t *ctx = zctx_new ();

    //  Initialize virtual transport interface
    vtx_t *vtx = vtx_new (ctx);
    int rc = vtx_register (vtx, "udp", vtx_udp_driver);
    assert (rc == 0);

    //  Create server socket and bind to all network interfaces
    void *server = vtx_socket (vtx, ZMQ_DEALER);
    assert (server);
    rc = vtx_bind (vtx, server, "udp://*:32000");
    assert (rc == 0);

    while (!zctx_interrupted) {
        char *input = zstr_recv (server);
        if (!input)
            break;              //  Interrupted
        free (input);
        zstr_send (server, "acknowledge");
        zclock_log ("S: acknowledge");
    }
    vtx_destroy (&vtx);

    zctx_destroy (&ctx);
    return 0;
}