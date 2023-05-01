
#ifndef RECOMMENDERSYSTEMLOADER_H
#define RECOMMENDERSYSTEMLOADER_H

#define ERR_MSG_OPEN_FILE "Error: failed to open the given file"
#define ERR_MSG_READ_FILE "Error: failed to read the given file"
#define NAME_YEAR_DELIMITER '-'
#define ERR_MSG_INVALID_FEAT_VAL \
"Error: invalid input, feature value should be a number between 0 to 10"
#define MIN_VAL 0
#define MAX_VAL 10
#include "RecommenderSystem.h"

class RecommenderSystemLoader {

 private:

 public:
  RecommenderSystemLoader () = delete;
  /**
   * loads movies by the given format for movies with their feature's score
   * @param movies_file_path a path to the file of the movies
   * @return smart pointer to a RecommenderSystem which was created with
   * those movies
   */
  static std::unique_ptr<RecommenderSystem> create_rs_from_movies_file
      (const std::string &movies_file_path) noexcept (false);

};

#endif //RECOMMENDERSYSTEMLOADER_H
