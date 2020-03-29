#include "play/lichess_interactor.h"

#include <boost/python.hpp>

BOOST_PYTHON_MODULE(bubikopf) {
  boost::python::class_<Chess::LichessInteractor>("LichessInteractor",
                                                  boost::python::init<>())
      .def("StartGame", &Chess::LichessInteractor::StartGame)
      .def("RespondTo", &Chess::LichessInteractor::RespondTo);
}
