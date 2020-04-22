#pragma once

#include "../application.h"
#include "set.h"
#include "set_updater.h"
#include "projects_tagger.h"
#include "users_tagger.h"
#include "set_writer.h"

/*************************************************************************
 * This computes the collaborators of Linus Torvalds.
 * is the linus example using the adapter.  And slightly revised
 *   algorithm that only ever trades the deltas.
 **************************************************************************/
class Linus : public Application {
public:
	int DEGREES = 4;  // How many degrees of separation form linus?
	int LINUS = 4967;   // The uid of Linus (offset in the user df)
	const char* PROJ = "datasets/projects.ltgt";
	const char* USER = "datasets/users.ltgt";
	const char* COMM = "datasets/commits.ltgt";
	DataFrame* projects; //  pid x project name
	DataFrame* users;  // uid x user name
	DataFrame* commits;  // pid x uid x uid
	Set* uSet; // Linus' collaborators
	Set* pSet; // projects of collaborators

	Linus(NodeConfiguration* conf): Application(conf) {}

	/** Compute DEGREES of Linus.  */
	void run() override {
		readInput();
		for (size_t i = 0; i < DEGREES; i++) step(i);
	}

	/** Node 0 reads three files, containing projects, users and commits, and
	 *  creates three dataframes. All other nodes wait and load the three
	 *  dataframes. Once we know the size of users and projects, we create
	 *  sets of each (uSet and pSet). We also output a data frame with a the
	 *  'tagged' users. At this point the dataframe consists of only
	 *  Linus. **/
	void readInput() {
		Key pK("projs");
		Key uK("usrs");
		Key cK("comts");
		if (this->getNodeId() == 0) {
			pln("Reading...");
			projects = this->fromFile(new Key(pK), PROJ);
			p("    ").p(projects->nRows()).pln(" projects");
			users = this->fromFile(new Key(uK), USER);
			p("    ").p(users->nRows()).pln(" users");
			commits = this->fromFile(new Key(cK), COMM);
			p("    ").p(commits->nRows()).pln(" commits");
			// This dataframe contains the id of Linus.
			delete this->fromScalar(new Key("users-0-0"), LINUS);
		} else {
			projects = this->waitAndGet(&pK);
			users = this->waitAndGet(&uK);
			commits = this->waitAndGet(&cK);
		}
		uSet = new Set(users);
		pSet = new Set(projects);
	}

	/** Performs a step of the linus calculation. It operates over the three
	 *  dataframes (projects, users, commits), the sets of tagged users and
	 *  projects, and the users added in the previous round. */
	void step(int stage) {
		p("Stage ").pln(stage);
		// Key of the shape: users-stage-0
		Key uK(StrBuff().c("users-").c(stage).c("-0").get());
		// A df with all the users added on the previous round
		DataFrame* newUsers = this->waitAndGet(&uK);
		Set delta(users);
		SetUpdater upd(delta);
		newUsers->map(&upd); // all of the new users are copied to delta.
		delete newUsers;
		ProjectsTagger ptagger(delta, *pSet, projects);
		commits->localMap(&ptagger); // marking all projects touched by delta
		merge(ptagger.newProjects, "projects-", stage);
		pSet->union_(ptagger.newProjects); //
		UsersTagger utagger(ptagger.newProjects, *uSet, users);
		commits->localMap(&utagger);
		merge(utagger.newUsers, "users-", stage + 1);
		uSet->union_(utagger.newUsers);
		p("    after stage ").p(stage).pln(":");
		p("        tagged projects: ").pln(pSet->size());
		p("        tagged users: ").pln(uSet->size());
	}

	/** Gather updates to the given set from all the nodes in the systems.
	 * The union of those updates is then published as dataframe.  The key
	 * used for the otuput is of the form "name-stage-0" where name is either
	 * 'users' or 'projects', stage is the degree of separation being
	 * computed.
	 */
	void merge(Set& set, char const* name, int stage) {
		if (this->getNodeId() == 0) {
			for (size_t i = 1; i < this->numNodes_; ++i) {
				Key nK(StrBuff().c(name).c(stage).c("-").c(i).get());
				DataFrame* delta = this->waitAndGet(&nK);
				p("    received delta of ").p(delta->nRows())
						.p(" elements from node ").pln(i);
				SetUpdater upd(set);
				delta->map(&upd);
				delete delta;
			}
			p("    storing ").p(set.size()).pln(" merged elements");
			SetWriter writer(set);
			Key k(StrBuff().c(name).c(stage).c("-0").get());
			delete this->fromVisitor(&k,"I", &writer);
		} else {
			p("    sending ").p(set.size()).pln(" elements to master node");
			SetWriter writer(set);
			Key k(StrBuff().c(name).c(stage).c("-").c(this->getNodeId()).get());
			delete this->fromVisitor(&k, "I", &writer);
			Key mK(StrBuff().c(name).c(stage).c("-0").get());
			DataFrame* merged = this->waitAndGet(&mK);
			p("    receiving ").p(merged->nRows()).pln(" merged elements");
			SetUpdater upd(set);
			merged->map(&upd);
			delete merged;
		}
	}
}; // Linus