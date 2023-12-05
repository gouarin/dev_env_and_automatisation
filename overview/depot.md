# Configurer son dépôt GitHub

:::{hint}Objectifs de cette partie
- Imposer des contraintes sur son dépôt GitHub pour le bien de tous
- Donner envie d'essayer son application ou sa librairie
- Faciliter les échanges avec les utilisateurs
:::

GitHub offre de nombreuses possibilités de configuration du dépôt et nous ne rentrerons pas ici dans la description de chacune d'elles. L'idée ici est d'en présenter quelques-unes qui ont un effet positif sur la mise en place du cadre de développement. Dans la suite de l'atelier, nous allons, au travers de GitHub Actions, mener un certain nombre de tests afin de vérifier la validité et la qualité du dépôt à chaque modification de celui-ci.

## Protection de la branche principale

Le projet ne peut évoluer que si la qualité est préservée. En d'autres termes, il ne devrait pas être possible de modifier la branche principale sans s'être assuré que rien n'est cassé. Il est donc fortement recommandé de protéger sa branche principale (`main` ou `master`) et de pouvoir la modifier qu'au travers de pull requests.

Pour ce faire, il suffit d'aller dans les `Settings` du dépôt puis dans `Branches` et enfin ajouter une règle. Il vous suffit ensuite de cocher la case `Require a pull request before merging` et de cliquer sur le bouton `Create`.

```{image} figures/protect_main.png
:width: 100%
:align: center
```

## Conserver un historique propre

Dans la section précédente , nous avons fait en sorte de ne travailler que par pull requests pour mettre à jour la branche principale. Nous pouvons aller un peu plus loin en essayant de conserver un historique des modifications le plus propre possible. Le choix fait ici est peut-être discutable pour certain mais il aura le mérite d'y réfléchir.

Lorsque l'on travaille par pull request, il n'est pas rare que l'on fasse plein de petits commits afin de résoudre la CI, revoir son implémentation ou tout simplement corriger des erreurs de typographie. On se retrouve alors avec un historique comportent une part non négligeable de commits qui n'ont pas grand rapport avec la correction d'un bug ou l'ajout d'une nouvelle fonctionnalité.

GitHub offre différentes manières de remettre le travail fait dans une pull request dans la branche principale :

- merge commits
  Tous les commits sont injectés à la branche de destination et ainsi toute l'historique de la pull request est conservée.
- squash merging
  Tous les commits sont fusionnés en un seul commit puis ce commit est inséré à la branche de destination.
- rebase merging
  Nous n'en parlerons pas ici, mais vous pouvez trouver plus de renseignements ici: [rebase merging](https://docs.GitHub.com/pull-requests/collaborating-with-pull-requests/incorporating-changes-from-a-pull-request/about-pull-request-merges#rebase-and-merge-your-commits)

Nous souhaitons pour la suite que les pull requests soient insérés dans la branche principale en un seul et unique commit représentant l'ensemble du travail fait. Bien évidemment, ça oblige également de ne pas faire des pull requests trop importantes. C'est en soi une bonne habitude à prendre.

Pour modifier le comportement des pull requests, il suffit d'aller dans les `Settings` du dépôt puis dans `General`. Vous verrez une partie `Pull Requests` que vous pouvez modifier comme suit

```{image} figures/pull_request.png
:width: 100%
:align: center
```

Nous en profiterons pour effacer automatiquement la branche qui a servi à la pull request une fois que celle-ci a été insérée à la branche principale en cochant la dernière case de la section `Pull Requests`.

:::{tip}Choix du message
Vous pouvez choisir quel message vous souhaitez pour le commit du *squash*. Nous avons choisi ici de prendre le titre de la pull request ainsi que sa description. Nous utiliserons ce message pour la construction du *changelog* en utilisant.
:::

## Utilisation de commits conventionnels

Lorsque nous faisons une nouvelle version d'un projet, il est bien de l'accompagner d'un journal des modifications également appelé **changelog**. Celui-ci permet en un coup d'œil de voir ce qui a évolué depuis la dernière version et ce aussi bien pour les développeurs que pour les utilisateurs. Ce journal peut contenir les informations suivantes : la correction des bugs, l'ajout de nouvelles fonctionnalités, l'ajout de documentation, l'ajout de nouveaux tests, ...

Voici un exemple de changelog pris sur le site de [pandas](https://pandas.pydata.org/docs/whatsnew/v2.1.3.html)

```{image} figures/changelog_pandas.png
:align: center
```

Les commits sont rangés selon des catégories. Nous retrouvons le titre de la pull request associée ainsi qu'un lien vers celle-ci. Vous pourriez reprendre l'ensemble des commits faits depuis la dernière version et faire le changelog à la main, mais comment catégoriser chacun des commits ? Si vous avez une centaine de commits à mettre dans le changelog, il se peut que ce soit nerveusement difficile !

Là encore, nous pouvons mettre un cadre qui fait que la génération du changelog est automatique et s'appuie sur les titres des pull requests qui auront été injectées dans la branche principale.

Ce cadre est mis en place à partir de la règle des [commit conventionnels](https://www.conventionalcommits.org). L'idée est d'apporter de l'information dans le titre d'un commit ou de la pull request pour catégoriser les changements qui sont faits. Cette convention suit également [SemVer](https://semver.org/lang/) que nous verrons un peu plus tard dans l'atelier et qui nous permettra d'incrémenter la version de notre application ou de notre librairie en fonction de la nature des changements effectués.

Le message du commit doit être structuré comme suit

```text
<type>[étendue optionnelle]: <description>

[corps optionnel]

[pied optionnel]
```

Le type définit la catégorie du changement. Il peut prendre les valeurs

```text
'build',
'chore',
'ci',
'docs',
'feat',
'fix',
'perf',
'refactor',
'revert',
'style',
'test'
```

L'étendue qui est optionnelle permet de donner un peu plus de détail sur la partie du projet qui est impactée. Nous donnons ici quelques exemples qui sont directement pris du site https://www.conventionalcommits.org.

- Message du commit avec description et breaking change dans le pied

    ```text
    feat: allow provided config object to extend other configs

    BREAKING CHANGE: `extends` key in config file is now used for extending other config files
    ```

- Message du commit sans corps

    ```text
    docs: correct spelling of CHANGELOG
    ```

- Message du commit avec étendue

    ```text
    feat(lang): add polish language
    ```
Nous verrons lors de l'utilisation de GitHub Actions comment vérifier que le message de la pull request suit bien cette convention et comment générer automatiquement le changelog et le numéro de montée en version lors de la création d'une nouvelle version.

:::{note} Remarque
L'utilisation des commits conventionnels permet d'une certaine manière de faciliter la communication entre les différents acteurs qui gravitent autour du développement open source de votre projet. Il en existe bien d'autres et c'est l'objet du chapitre [](communication).
:::

## Autres fonctionnalités

Vous pouvez également configurer d'autres fonctionnalités qui permettent aux visiteurs de ne pas se tromper de chemin. Par exemple, si votre documentation est sur [Read the Docs](https://readthedocs.org) ou générée via [GitHub Pages](https://pages.GitHub.com/), il n'est peut-être pas nécessaire d'avoir un `wiki`.

Vous pouvez également avoir envie d'avoir un espace où les personnes peuvent échanger entre elles soit sous la forme d'un forum comme le propose [discourse](https://www.discourse.org/), soit sous la forme d'un chat comme le propose [discord](https://discord.com/) ou [element](https://element.io/). Si vous cherchez la première forme, vous devriez vous pencher sur [GitHub Discussions](https://docs.GitHub.com/fr/discussions).

Dans tous les cas, vous pouvez retourner dans les `Settings` de votre dépôt dans la partie `General`, il y a une section `Features`. Nous allons ici retirer le `wiki` et activer les discussions.

```{image} figures/discussions.png
:align: center
```

Dans la barre du haut, vous devriez voir disparaître l'onglet `Wiki` et apparaître l'onglet `Discussions`.
