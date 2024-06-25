#pragma once

#include "sigZone.hpp"
#include "../visitor/graphVisitor.hpp"
#include "thread.hpp"

namespace namu {
    template <typename W>
    class _nout defaultSigZone : public sigZone {
        NAMU(ME(defaultSigZone, sigZone))

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

    private:
        void _logFrames();
    };
}
