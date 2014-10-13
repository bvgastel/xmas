XMAS
====

This project is a design tool for NoC system (Network on Chip).
It uses the language Intel detailed in papers concerning xmas 

TODO: add details of these papers.


Contents
========

- minutes			The minutes, only the TeX sources.
- preparation	 	Files used during preparation plus plan
- xmas-info			Information on xmas (pdf)


Use
===

We use git for communication with the following use cases

1. New repo
	Clone the github directory using git as described below.
	*nix users from a bash shell, ms windows from git bash.

	machine/xmas> git clone https://github.com/gbonnema/clone

	If you can become collaborator, you should be able to push
	your updates to the same URL.

	If you cannot, then you could fork the repo and do a pull 
	request.

2. Creating a new branch.
	You create a new branch "plan" with the command

	* git checkout -b plan
	
	do the alterations and commits until the modifications are finished
	then push the commits to remote

	* git push --set-upstream origin plan

	If you forget the option "--set-upstream" git will issue an error
	because it does not recognize the branch yet. Once the branch
	is created on remote with "--set-upstream", you can do a "git push".

2. Collaborator
	Clone the repo, use it as you normally would with
	change-commit cycles and when you want to publish do a
	git push. If you set up git remote properly, then 
	the github repo should be updated.

	If you want to update your working directory with any
	changes from github, make sure you committed and then do
	a git pull. Again, remember to set up git remote.

	Sometimes a "git push" will result in an error, because
	your working directory is not updated with the latest
	changes from remote. Make sure to do a "git pull", resolve
	any conflicts, do a commit and a push to get your committed 
	changes to remote.

3. Any github user
	If you are using the system and want to update it, you
	can clone the repo locally, make the changes with the
	usual change-commit cycles. When done (including
	documentation and tests) you can do a pull request.

Documentation
=============

We use two document formats: LaTeX and markdown. 

Project documentation
---------------------
We use LaTeX for most of the project documents like the plan, a tutorial or a guide. 
Any kind of document that needs to look good on paper and is more than just a screenful.

System documentation
--------------------
We use markdown with pandoc format for most of the system documentation 

Development process
===================

We use Disciplined Agile Delivery (DAD) for our process and use a cloud based
agile management tool to visualise our work and to keep track of the progress.

Compile and install
===================

Todo

Dependencies
============

Todo
