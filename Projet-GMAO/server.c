#include <libwebsockets.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define EXAMPLE_RX_BUFFER_BYTES (10)

// Callback pour gérer les événements WebSocket
static int callback_echo(struct lws *wsi, enum lws_callback_reasons reason,
                          void *user, void *in, size_t len) {
    switch (reason) {
    case LWS_CALLBACK_RECEIVE:  // Utiliser LWS_CALLBACK_RECEIVE pour les messages reçus
        printf("Message recu : %.*s\n", (int)len, (char *)in);
        // Écho du message au client
        lws_write(wsi, (unsigned char *)in, len, LWS_WRITE_TEXT);
        break;
    case LWS_CALLBACK_ESTABLISHED:
        printf("Client connecte\n");
        break;
    case LWS_CALLBACK_CLOSED:
        printf("Client déconnecte\n");
        break;
    default:
        break;
    }
    return 0;
}

// Définir l'interface du protocole WebSocket
static struct lws_protocols protocols[] = {
    {
        "example-protocol",     // Nom du protocole
        callback_echo,          // Callback du protocole
        0,                      // Pas d'options spéciales
        EXAMPLE_RX_BUFFER_BYTES // Taille du buffer de réception
    },
    { NULL, NULL, 0, 0 } // Terminer le tableau
};

int main(void) {
    struct lws_context_creation_info info;
    struct lws_context *context;
    memset(&info, 0, sizeof(info));
    
    info.port = 9001;  // Port d'écoute
    info.protocols = protocols;

    context = lws_create_context(&info);

    if (!context) {
        fprintf(stderr, "Échec de la création du contexte\n");
        return 1;
    }

    // Boucle principale du serveur
    while (1) {
        lws_service(context, 0);
    }

    lws_context_destroy(context);
    return 0;
}