/// @file
#pragma once

#include "core/worker/visitor/graphVisitor.hpp"
#include "core/frame/sigZone.hpp"
#include "core/frame/thread.hpp"

namespace by {
    template <typename W> class defaultSigZone: public sigZone {
        BY(ME(defaultSigZone, sigZone))

    public:
        defaultSigZone(const W& worker):
            super([&](const baseErr& e) {
                enablesZone zone;
                zone.setEnable(true);

                logger& log = logger::get();
                log.logBypass("\n* * *\n");
                log.logBypass("unexpected exception found: ");
                e.dump();

                if(worker.isFlag(W::LOG_STRUCTURE)) {
                    log.logBypass("\ncurrent frames:\n");
                    thread::get().getFrames();
                }

                if(worker.isFlag(W::LOG_GRAPH_ON_EX)) {
                    log.logBypass("\ngraph:\n");
                    graphVisitor().setFlag(0).setTask(worker.getTask()).work();
                }

                // signale will *terminate* the process after all.
            }) {}
    };
}
