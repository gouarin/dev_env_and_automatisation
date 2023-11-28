---
title: Cadre de développement et automatisation pour l'open source
---

Le développement de logiciels open source en simulation numérique prend une part de plus en plus importante dans le processus de recherche. Il permet de valider les méthodes numériques, de développer de nouveaux algorithmes et de permettre à n’importe qui de s’approprier les méthodes et de les enrichir. Ouvrir nos codes de recherche fait également partie de la démarche de science ouverte et il ne s’agit pas de “juste” mettre une archive avec les sources sur une page web.

La philosophie et la structure d’un logiciel émanent de ses précurseurs et évoluent avec le temps au fil des différentes contributions. Néanmoins, pour que cette architecture ne s’effondre pas au moindre changement, un cadre est nécessaire. La mise en place d’un tel cadre est une contrainte. Il s’agit d’accompagner les contributeurs dans ces nouvelles pratiques et de trouver le compromis entre cadre contraignant et laisser libre cours à la créativité.

Lors du processus de développement, de nombreuses tâches sont répétitives et peuvent être source d’erreurs ou de régressions de l’application. On peut parler ici des tests unitaires, mais également du formatage de code, des erreurs pouvant être révélées lors de l’analyse statique, de la génération de la documentation, de la création de nouvelles versions avec la distribution de l’application via des systèmes de packaging (pypi, conda, vcpkg, spack, nix, guix, …).

La première demi-journée sera constituée d’interventions qui présentent les différentes étapes d’un développement de logiciel open source en discutant du cadre pertinent à mettre en place. Un temps d’échange avec les participants clôturera cette première partie. Le seconde demi-journée sera plus en mode TP où il sera proposé aux participants de manipuler les github actions pour l’automatisation de ces tâches répétitives. La mise en place et l'exécution d'un processus similaire sur un serveur gitlab couplée à une infrastructure d'exécution de tests seront également proposés sous forme de démonstration.

## Objectifs

- Améliorer et sécuriser son processus de développement
- Diffuser son travail de manière semi automatique et l’ouvrir au plus grand nombre
- Comprendre le fonctionnement de github actions
- Acquérir une autonomie pour mettre en place ses propres workflows
