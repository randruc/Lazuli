;; SPDX-License-Identifier: GPL-3.0-only
;; This file is part of Lazuli.
;; Copyright (c) 2020, Remi Andruccioli <remi.andruccioli@gmail.com>

;;; Directory Local Variables for Emacs
;;; For more information see (info "(emacs) Directory Variables")

;; Define the command to run on "compile" from Emacs.
;;
;; According to the documentation of docker exec: "COMMAND will run in the
;; default directory of the container. If the underlying image has a custom
;; directory specified with the WORKDIR directive in its Dockerfile, this will
;; be used instead".
;; https://docs.docker.com/engine/reference/commandline/exec/
((c-mode
  (compile-command . "podman exec lazuli bash -c ./.compile.sh ")))
