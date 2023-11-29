# La communication

La communication est un élément essentiel pour que le projet prenne de l'ampleur et qu'une communauté se crée. Elle doit tenir compte de chacune des personnes susceptibles d'apporter sa contribution. Celle-ci peut-être de différentes natures : indiquer un bug, demander une nouvelle fonctionnalité, poser une question, corriger un bug, faire de la documentation, ajouter une nouvelle fonctionnalité, relire les pull requests et les valider, ...

Comment faire en sorte que l'ensemble de ces étapes qui sont inhérentes à chaque développement open-source se passent au mieux ? Comment faire pour donner une certaine autonomie aux acteurs pour qu'ils se sentent investis et créatifs dans un cadre contraint ?

Si nous y réfléchissons un peu, nous n'aimons généralement pas ne pas savoir par où commencer surtout lorsque nous prenons le travail de quelqu'un d'autre. Malgré la persévérance de certain, il n'est pas rare de voir des acteurs potentiels se décourager et aller voir chez quelqu'un d'autre. Mais, si ça se trouve, il ne manquait pas grand-chose : une documentation, une information, ...

Nous allons voir dans la suite différents documents et outils qui permettent aux acteurs de se sentir bien et en confiance. Ils savent qu'ils trouveront la réponse à leurs interrogations et, si ce n'est pas le cas, ils sauront où s'adresser pour l'avoir.

:::{attention}La langue
Si vous voulez toucher le plus de monde possible, il est fortement recommandé d'écrire en anglais : les commentaires dans le code, la documentation, les messages des commits, ...
:::

## Qui a le droit d'utiliser ce logiciel ?

Pour que votre logiciel open-source puisse être utilisé, il doit contenir une licence qui décrit dans quelles conditions celui-ci peut être utilisé. Le choix d'une licence est un vaste sujet et dépasse largement le cadre de cet atelier. Nous ne donnerons ici que quelques pointeurs qui vous permettront d'approfondir la question puis nous expliquerons la mise en œuvre dans le dépôt github.

:::{tip} Ressources aidant au choix d'une licence
- [Liste des licences](https://opensource.org/licenses/)
- [The Legal Side of Open Source](https://opensource.guide//legal/)
- [Science ouverte : codes et logiciels](https://www.ouvrirlascience.fr/wp-content/uploads/2022/10/Passeport_Codes-et-logiciels_WEB.pdf)
- [The Software Package Data Exchange (SPDX)](https://spdx.dev/)
:::

Nous rappelons également qu'il est important de se rapprocher de son employeur et de le consulter pour la mise en ligne d'un projet open source.

Pour ajouter une licence à votre dépôt, il vous suffit d'ajouter un fichier `LICENSE` ou `LICENSE.md` qui contient le contenu légal de la licence que vous aurez choisie. Prenons par exemple le contenu de la licence MIT qui est assez court

> Copyright `<YEAR> <COPYRIGHT HOLDER>`
>
> Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
>
> The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
>
> THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Vous pouvez le faire manuellement : c'est-à-dire créer le fichier localement, ouvrir une pull request et enfin insérer celle-ci dans la branche principale. Vous pouvez également le faire au travers de l'interface de github [Adding a license to a repository](https://docs.github.com/en/communities/setting-up-your-project-for-healthy-contributions/adding-a-license-to-a-repository).

## Que fait ce logiciel ?

Lorsque vous avez besoin de savoir si un logiciel existe pour vos besoins, que faites-vous ? Probablement que vous allez faire une recherche sur votre moteur de recherche préféré, il se peut aussi que vous alliez directement sur github pour taper quelques mots clés et voir si vous trouvez votre bonheur. Quoi qu'il en soit, la première page que vous allez trouver sera certainement la page d'un dépôt github ou gitlab. Il est donc important que celle-ci donne assez de renseignements pour maintenir la lectrice ou le lecteur en haleine.

Sur github, les informations affichées sur un dépôt sont celles contenues dans le fichier `readme`.

:::{note}
Un fichier de `readme` peut avoir plusieurs extensions `.md`, `.rst` ou sans extension.
:::

Si c'est la première vue de votre logiciel, vous comprendrez bien qu'il est important de ne pas en négliger sa rédaction. Il existe différents sites qui vous aident sur l'élaboration de son contenu.

:::{tip}Ressources pour écrire un `readme`
- [Make a README](https://www.makeareadme.com/)
- [Awesome README](https://github.com/matiassingers/awesome-readme)
- [GitHub README Templates](https://www.readme-templates.com/)
:::

Voici quelques éléments qui devraient faire partie de votre `readme`:

- Le nom du projet
- Une descritpion du projet
- Des badges permettant d'accéder facilement à la documentation, aux tests, aux packages, ...
- Un guide sur l'installation
- Un exemple d'utilisation
- Où aller pour plus d'informations
- Comment contribuer
- Une feuille de route du projet (où nous souhaitons aller)

## Comment installer ce logiciel ?

Il est plus facile d'essayer un logiciel lorsque nous n'avons rien à faire. S'il faut installer dix mille dépendances qui ne sont même pas documentées et que, même après cette étape, vous n'arrivez pas à installer le logiciel, car il y a un problème de version, vous allez aller voir ailleurs et c'est bien dommage pour celui qui a développé ce logiciel génial, mais qui n'a pas pris le temps de prendre soin du packaging.

Si l'installation est bien documentée (même si c'est à partir des sources), que le processus n'a pas l'air trop contraignant, alors il y a de forte chance que la personne essaie. Bien évidemment, le logiciel évolue et les dépendances avec lui. Il serait bon que cette partie soit testée. C'est ce que nous ferons par la suite en utilisant github-action.

Nous pouvons aller encore plus loin en ayant le logiciel sur un système de gestion de paquets (yarn, cargo, conda, ...). La personne n'a plus qu'à utiliser sa ligne de commande préférée pour installer le logiciel.

Il existe également des solutions où il n'y a qu'à cliquer sur une url et se laisser guider. Parmi les outils qui existent, nous pouvons citer

- [solara](https://solara.dev/)
- [streamlit](https://streamlit.io/)
- [voilà](https://voila-gallery.org/)

Ils permettent de développer rapidement des tableaux de bord à l'aide de notebook Jupyter ou autres où nous pouvons tester le logiciel. Le déploiement se fait directement sur des services cloud (streamlit, [binder](https://mybinder.org/), [Ploomber Cloud](https://www.platform.ploomber.io/), [netlify](https://www.netlify.com/),...). Il est de plus possible d'avoir accès à un terminal avec le logiciel installé et les exemples compilés.

## Comment se servir de ce logiciel ?

La deuxième étape (une fois que la personne veut en savoir un peu plus) repose sur la documentation


## Une question, un problème ?

## Une contribution ?

## Les règles de bonne conduite



<!-- github discussion -->