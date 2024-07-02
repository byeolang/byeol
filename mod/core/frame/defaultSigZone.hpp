#pragma once

#include "sigZone.hpp"
#include "../visitor/graphVisitor.hpp"
#include "thread.hpp"

namespace nm {
    template <typename W>
    class defaultSigZone : public sigZone {
        NM(ME(defaultSigZone, sigZone))

    public:
        defaultSigZone(const W& worker): super([&](const err& e) {
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