---
title: Cadre de développement et automatisation pour l'open source
---

Le développement de logiciels open source en simulation numérique prend une part de plus en plus importante dans le processus de recherche. Il permet de valider les méthodes numériques, de développer de nouveaux algorithmes et de permettre à n’importe qui de s’approprier les méthodes et de les enrichir. Ouvrir nos codes de recherche fait également partie de la démarche de science ouverte et il ne s’agit pas de “juste” mettre une archive avec les sources sur une page web.

La philosophie et la structure d’un logiciel émanent de ses précurseurs et évoluent avec le temps au fil des différentes contributions. Néanmoins, pour que cette architecture ne s’effondre pas au moindre changement, un cadre est nécessaire. La mise en place d’un tel cadre est une contrainte. Il s’agit d’accompagner les contributeurs dans ces nouvelles pratiques et de trouver le compromis entre cadre contraignant et laisser libre cours à la créativité.

Lors du processus de développement, de nombreuses tâches sont répétitives et peuvent être source d’erreurs ou de régressions de l’application. On peut parler ici des tests unitaires, mais aussi du formatage de code, des erreurs pouvant être révélées lors de l’analyse statique, de la génération de la documentation, de la création de nouvelles versions avec la distribution de l’application via des systèmes de packaging (pypi, conda, vcpkg, spack, nix, guix, …).

Ce document est le contenu d'un atelier proposant de réfléchir à l'ensemble des actions pouvant rendre la vie des différents acteurs (mainteneurs, développeurs, utilisateurs...) plus agréable. Nous utiliserons GitHub Actions pour l’automatisation de certaines de ces tâches répétitives.

## Objectifs

- Améliorer et sécuriser son processus de développement
- Diffuser son travail de manière semi-automatique et l’ouvrir au plus grand nombre
- Comprendre le fonctionnement de GitHub Actions
- Acquérir une autonomie pour mettre en place ses propres workflows

## Programme

Ce document est constitué de deux parties

- Un tour d'horizon qui essaie de décrire les différents étapes et processus qui permettent à un logiciel open source d'acquérir une certaine maturité au niveau de sa qualité. La mise en place d'un cadre de développement apparaît comme inévitable pour permettre à chacun des acteurs du développement logiciel d'avoir sa place. Pendant ce tour, nous parlerons d'un certain nombre d'outils qui sont là pour nous faciliter la tâche. Nous avons essayé, dans la mesure du possible, de les rendre agnostique au langage de programmation utilisé.
- Une mise en pratique qui permet d'appliquer dans un cas réel l'ensemble des notions abordées dans la première partie où nous nous appuierons sur l'intégration continue et plus particulièrement sur GitHub Actions. Nous verrons ainsi comment mettre de manière automatique un cadre de développement évitant, ou du moins limitant, ainsi les erreurs et les inattentions humaines.

## Matériel

L'ensemble du matériel pour la session pratique est sur le dépôt GitHub de cet atelier : https://github.com/gouarin/dev_env_and_automatisation.

Pour chaque partie, vous trouverez une correction dans le répertoire `material`. Si des fichiers sont nécessaires pour réaliser les exercices, ils se trouveront également dans le répertoire `material` dans leur partie et dans un répertoire appelé `required_files`.

## Rendu final

Si vous êtes impatient et que vous voulez voir à quoi ressemble un projet prenant en compte l'ensemble de ces notions, vous pouvez aller voir le projet **splinart-cpp** en cliquant sur l'image.

<a href="https://github.com/gouarin/splinart-cpp">

:::{figure} logo/dark_logo.png
:::

</a>

Si vous avez apprécié le contenu de ce document, n'hésitez pas à mettre une petite étoile !!

## Remerciements

Un grand merci à [Matthieu Haefele](https://github.com/mathaefele) pour sa relecture attentive et pour avoir été le premier à expérimenter le matériel.
