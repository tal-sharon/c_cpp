#ifndef SCHOOL_SOLUTION_USERFACTORY_H
#define SCHOOL_SOLUTION_USERFACTORY_H

#include "RSUser.h"
#define ERR_MSG_OPEN_FILE "Error: failed to open the given file"
#define ERR_MSG_READ_FILE "Error: failed to read the given file"
#define ERR_MSG_DUPLICATED_KEY \
"Error: invalid input, got two different movies with same name."
#define ERR_MSG_INVALID_RANK \
"Error: invalid input, rank should be a number between 0 to 10"
#define MIN_RANK 0
#define MAX_RANK 10
#define USER_NAME_INDEX -1
#define NAME_YEAR_DELIMITER '-'
#define SPACE_DELIMITER ' '

class RSUsersLoader
{
 private:

 public:
  RSUsersLoader() = delete;
  /**
   *
   * loads users by the given format with their movie's ranks
   * @param users_file_path a path to the file of the users
   *                        and their movie ranks
   * @param rs RecommendingSystem for the Users
   * @return vector of the users created according to the file
   */
  static std::vector<RSUser> create_users_from_file(const std::string&
  users_file_path, std::unique_ptr<RecommenderSystem> rs) noexcept(false);
};


#endif //SCHOOL_SOLUTION_USERFACTORY_H
