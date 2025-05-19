#include <libwebsockets.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define EXAMPLE_RX_BUFFER_BYTES (100)

// Fonction pour écrire dans le CSV
void ecrire_csv(const char *message) {
    printf("Ecriture dans CSV : %s\n", message);
    
    FILE *f = fopen("C:\\Users\\utilisateur\\Desktop\\EPF\\PAML\\Projet GMAO\\Projet-GMAO\\donnees.csv", "w");
    if (f == NULL) {
        perror("Erreur ouverture fichier");
        return;
    }
    fprintf(f, "%s\n", message); // écrit le message + saut de ligne
    fclose(f);
}

void lire_fichier_temporaire(struct lws *wsi) {
    FILE *f = fopen("C:\\Users\\utilisateur\\Desktop\\EPF\\PAML\\Projet GMAO\\Projet-GMAO\\fichier_temporaire.csv", "r");
    if (f == NULL) {
        perror("Erreur lors de l'ouverture de fichier_temporaire.csv");
        return;
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), f)) {
        printf("Ligne : %s", ligne);

        size_t len = strlen(ligne);
        if (len > 0 && ligne[len-1] == '\n') {
            ligne[len-1] = '\0'; // retirer saut de ligne
            len--;
        }

        unsigned char buffer[LWS_PRE + 256];
        memcpy(&buffer[LWS_PRE], ligne, len);
        lws_write(wsi, &buffer[LWS_PRE], len, LWS_WRITE_TEXT);
    }

    fclose(f);
}

// Callback WebSocket
static int callback_echo(struct lws *wsi, enum lws_callback_reasons reason,
                         void *user, void *in, size_t len) {
    switch (reason) {
        case LWS_CALLBACK_RECEIVE: {
            char message[EXAMPLE_RX_BUFFER_BYTES];
            snprintf(message, sizeof(message), "%.*s", (int)len, (char *)in);

            printf("Message recu : %s\n", message);
            ecrire_csv(message);

            // Lancement du script VBS pour exécuter la macro dans Excel
            system("cscript //nologo C:\\Users\\utilisateur\\Desktop\\EPF\\PAML\\runExcel.vbs");

            // Envoie les infos du fichier temporaire au client
            lire_fichier_temporaire(wsi);

            // Répondre au client avec le même message (optionnel)
            //lws_write(wsi, (unsigned char *)message, strlen(message), LWS_WRITE_TEXT);

            break;
        }
        case LWS_CALLBACK_ESTABLISHED:
            printf("Client connecté\n");
            break;
        case LWS_CALLBACK_CLOSED:
            printf("Client déconnecté\n");
            break;
        default:
            break;
    }
    return 0;
}

// Protocole WebSocket
static struct lws_protocols protocols[] = {
    {
        "example-protocol",
        callback_echo,
        0,
        EXAMPLE_RX_BUFFER_BYTES
    },
    { NULL, NULL, 0, 0 }
};

int main(void) {
    struct lws_context_creation_info info;
    struct lws_context *context;
    memset(&info, 0, sizeof(info));

    info.port = 9001;
    info.protocols = protocols;

    context = lws_create_context(&info);
    if (!context) {
        fprintf(stderr, "Échec de la création du contexte\n");
        return 1;
    }

    printf("Serveur démarré sur le port %d\n", info.port);

    while (1) {
        lws_service(context, 0);
    }

    lws_context_destroy(context);
    return 0;
}
