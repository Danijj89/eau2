#pragma once

#include "../reader.h"
#include "set.h"

/***************************************************************************
 * The UserTagger is a reader that is mapped over commits, and marks all of
 * the users which commmitted to a project to which a collaborator of Linus
 * also committed as an author. The commit dataframe has the form:
 *    pid x uid x uid
 * where the pid is the idefntifier of a project and the uids are the
 * identifiers of the author and committer.
 *************************************************************************/
class UsersTagger : public Reader {
public:
	Set& pSet;
	Set& uSet;
	Set newUsers;

	UsersTagger(Set& pSet,Set& uSet, DataFrame* users):
			pSet(pSet), uSet(uSet), newUsers(users->nRows()) { }

	bool visit(Row & row) override {
		int pid = row.getInt(0);
		int uid = row.getInt(1);
		if (pSet.test(pid))
			if(!uSet.test(uid)) {
				uSet.set(uid);
				newUsers.set(uid);
			}
		return false;
	}
};