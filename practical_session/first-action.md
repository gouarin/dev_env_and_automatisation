# Première action GitHub

Nous allons ici mettre en place la première action à exécuter par GitHub Actions. Nous allons voir si ce que nous avons réussi à faire fonctionner localement avec pixi fonctionne également dans l'intégration continue.

Vous allez commencer par créer une branche, car, rappelez-vous il est interdit de mettre des choses directement dans la branche principale. Nous voulons nous assurer que les changements soient le plus sain possible par rapport à notre cadre de développement.


## Premier workflow

Allons-y, créons cette nouvelle branche

```shell
git checkout -b first-action
```

Nous allons maintenant créer le fichier `.github/workflows/ci.yml` contenant

```yaml
name: "Run CI"

on: pull_request

jobs:
  tests:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3

```

Ajoutez ce fichier via un commit et pousser la branche sur votre dépôt GitHub.

Vous devriez être en mesure d'ouvrir une pull request à partir de cette branche. Une fois que c'est fait, vous devriez voir l'action se mettre en place, car nous avons demandé qu'elle se déclenche à chaque pull request. Pour plus de détail, vous pouvez par ailleurs aller dans l'onglet `Actions`.

## Mise en place des tests

Si tout s'est bien passé, ajoutez l'étape suivante

```yaml
- run: pixi run test
```

Ajoutez ces changements dans la pull request : il suffit de commiter ces changements localement et de les pousser.

L'intégration continue devrait échouer, car la commande `pixi` n'est pas reconnue. Il vous faut donc ajouter une étape : l'installation de pixi.

Vous pourriez le faire à la main, mais il vous faut savoir comment différencier l'installation entre OS et vous ne savez pas encore comment faire. Vous vous dites que vous allez peut-être trouver votre bonheur dans le [MarketPlace](https://github.com/marketplace?type=actions).

Recherchez s'il existe une action pour installer pixi et si oui, ajoutez cette étape à votre processus.

Vous devriez maintenant avoir une action qui construit et teste l'application sous ubuntu.

## Multi OS

Tout a l'aire de fonctionner sous linux, mais en est-ol de même sur les autres OS. Pour s'en assurer, nous décidons d'ajouter macos.

Pour cela, nous allons utiliser la notion de matrice dans GitHub Actions qui permet de lancer l'ensemble des étapes pour différentes configurations.
