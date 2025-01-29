Le développement de logiciels open source en simulation numérique prend une part de plus en plus importante dans le processus de recherche. Il permet de valider les méthodes numériques, de développer de nouveaux algorithmes et de permettre à n’importe qui de s’approprier les méthodes et de les enrichir. Ouvrir nos codes de recherche fait également partie de la démarche de science ouverte et il ne s’agit pas de “juste” mettre une archive avec les sources sur une page web.

La philosophie et la structure d’un logiciel émanent de ses précurseurs et évoluent avec le temps au fil des différentes contributions. Néanmoins, pour que cette architecture ne s’effondre pas au moindre changement, un cadre est nécessaire. La mise en place d’un tel cadre est une contrainte. Il s’agit d’accompagner les contributeurs dans ces nouvelles pratiques et de trouver le compromis entre cadre contraignant et laisser libre cours à la créativité.

Lors du processus de développement, de nombreuses tâches sont répétitives et peuvent être source d’erreurs ou de régressions de l’application. On peut parler ici des tests unitaires, mais aussi du formatage de code, des erreurs pouvant être révélées lors de l’analyse statique, de la génération de la documentation, de la création de nouvelles versions avec la distribution de l’application via des systèmes de packaging (pypi, conda, vcpkg, spack, nix, guix, …).

Ce répertoire est le contenu d'un atelier proposant de réfléchir à l'ensemble des actions pouvant rendre la vie des différents acteurs (mainteneurs, développeurs, utilisateurs, ...) plus agréable. Nous utiliserons GitHub Actions pour l’automatisation de certaines de ces tâches répétitives.

https://gouarin.github.io/dev_env_and_automatisation/
