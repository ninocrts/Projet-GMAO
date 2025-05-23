#include <libwebsockets.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define EXAMPLE_RX_BUFFER_BYTES (100)

// Fonction pour écrire dans la 2e ligne du CSV en gardant la 1ère ligne
void ecrire_csv(const char *message) {
    printf("Ecriture dans CSV : %s\n", message);

    // 1. Lire la 1ère ligne existante
    FILE *f_read = fopen("C:\\Users\\utilisateur\\Desktop\\EPF\\PAML\\Projet GMAO\\Projet-GMAO\\donnees.csv", "r");
    if (f_read == NULL) {
        perror("Erreur ouverture fichier en lecture");
        return;
    }

    char first_line[256] = "";
    if (fgets(first_line, sizeof(first_line), f_read) == NULL) {
        first_line[0] = '\0';  // fichier vide
    }
    fclose(f_read);

    // 2. Ouvrir en écriture (efface contenu)
    FILE *f_write = fopen("C:\\Users\\utilisateur\\Desktop\\EPF\\PAML\\Projet GMAO\\Projet-GMAO\\donnees.csv", "w");
    if (f_write == NULL) {
        perror("Erreur ouverture fichier en écriture");
        return;
    }

    // 3. Réécrire la 1ère ligne (si elle existe)
    if (strlen(first_line) > 0) {
        fprintf(f_write, "%s", first_line);
    } else {
        fprintf(f_write, "\n"); // sinon ligne vide
    }

    // 4. Écrire la nouvelle 2e ligne
    fprintf(f_write, "%s\n", message);

    fclose(f_write);
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

            system("cscript //nologo C:\\Users\\utilisateur\\Desktop\\EPF\\PAML\\runExcel.vbs");

            lire_fichier_temporaire(wsi);
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
