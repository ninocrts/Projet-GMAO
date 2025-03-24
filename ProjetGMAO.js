    // Fonction pour afficher une forme spécifique
    function afficherForme(forme) {
        // Masquer toutes les formes
        document.getElementById("cercleVert").classList.add("hidden");
        document.getElementById("carreauJaune").classList.add("hidden");
        document.getElementById("carreRouge").classList.add("hidden");

        // Afficher la forme sélectionnée
        document.getElementById(forme).classList.remove("hidden");
    }

