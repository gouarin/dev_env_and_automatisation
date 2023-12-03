# Plan de l'atelier

1. Créer un environnement de développement (C++)
    - conda
    - pixi
    - guix
    - spack

2. Configurer le dépôt
   - utilisation de [conventional commit](https://www.conventionalcommits.org/en/v1.0.0/)
   - protection de la branche main
   - ajout de la licence, du code de conduite, du readme, ...

3. utilisation de pre-commit pour
   - différents hooks sur le formatage de fichier
   - utilisation de clang-format
   - vérification de l'indentation

4. Présentation de GitHub Actions
    - différence avec les autres outils de CI
    - format d'une action
    - présentation du market place

5. 1ere CI
   - machine linux
   - installation de l'environnement de développement
   - lancement des 1er tests

6. Validation du titre de la PR au formalisme de conventional commit

7. Utilisation d'une matrice
    - compiler le projet sur plusieurs os
    - compiler le projet avec plusieurs compilo

8. Cron et mise en place de tests longs toutes les nuits

9.  Ajout du pre-commit et de cpp-check
   - les tests précédents sont exécutés uniquement si ces 2 premiers tests sont valides

10. Création de la documentation
   - mise en place de l'environnement de doc
   - construction de la doc via sphinx
   - sauvegarde du répertoire output sous forme d'artifact
   - mise en place de GitHub pages avec cet artifact

11. Création d'une release
   - GitHub Actions avec workflow_dispatch
   - fabrication de la release avec le change log
   - déclenchement de la construction du paquet conda et déploiement sur une channel conda